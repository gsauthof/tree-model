// Copyright 2016, Georg Sauthoff <mail@georg.so>

/* {{{ LGPLv3

    This file is part of tree-model.

    tree-model is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    tree-model is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with tree-model.  If not, see <http://www.gnu.org/licenses/>.

}}} */
#include "item_adaptor.hh"

#include "base.hh"

#include <QMimeData>
#include <QDebug>

#include <deque>

/*

The Item_Adaptor extends QAbstractItemModel and translates
its calls to tree_model::Base ones.

## QAbstractItemModel Overview

The idea of QAbstractItemModel is to provide an abstract base class
for a model that is flexible enough to implement list-like, table-like and
tree-like models.

The model returns item location information via QModelIndex values. A item can
be referenced via a QModelIndex value or a (row, column, QModelIndex) tuple.
A special QModelIndex value is used to denote an index being invalid.

A tree-like model, by convention, usually manages its hierarchical
information in column 0. That means that parent indexes returned
by the model always have column == 0 and requesting a child of
an element from column > 0 always returns an invalid QModelIndex.

A tree model usually is implemented in the following way:

    Call                         Semantics
    ----------------------------------------------------
    QModelIndex()                the invalid value,
                                 references the invisible root
    index(0, 0, QModelIndex())   index referencing the first visible root,
                                 i.e. the first child of the invisible root
                                 (usually there is just one,
                                  think DOM tree root)
    index(n, 0, p)               the 1st column of the n-th child of index p,
                                 e.g. its key (think element name)
    index(n, 1, p)               the 2nd column of the n-th child of index p,
                                 e.g. its value (think element content)
    rowCount(p)                  the number of children of p
    columnCount(p)               the number of columns of the children of p

A QModelIndex can be used to retrieve/change the data it references,
insert/remove child elements etc.

Properties that should hold:

    i.isValid() == false => parent(i).isValid() == false

    i.isValid() == false <=> (-1, -1, 0)

    Different columns of an element share the same parent:
    parent((r, c1, p)) == parent((r, c2, p))

    The column of an index returned by parent() is always 0:
    parent(i).column() == 0

    But, with index(), hierarchical information is only available
    in the first column:
    index(r, c, (pr, pc, pp)).isValid() == false   | for pc > 0

    index(r, c, (pr, 0, pp)).isValid() == true | if (pr, 0, pp) has > r children and each child has > c columns

    sibling(r, c, i) == index(r, c, parent(i))

    thus, in contrast to index(), sibling() does return hierachical information
    for columns > 0:
    sibling(r, c, (ir, 0, ip)).isValid() =>
    sibling(r, c, (ir, ic, ip)).isValid() | if 0 < ic < columnCount((ir, 0, ip))


## QModelIndex Overview

The QModelIndex class implements value semantics. It is a 4-tuple containing a
row, column and id values and a pointer to a model. A special QModelIndex value
denotes the invalid QModelIndex, i.e. (-1, -1, 0, 0). Validity can be tested
with the method isValid(), and freely constructed QModelIndex objects are
always invalid. Valid QModelIndex objects can only be obtained from a
QAbstractItemModel.

The id inside a QModelIndex can be a real id or a pointer that
identifies an item.

A QModelIndex provides some convenience methods that mirror (const)
QAbstractItemModel methods:

    QModelIndex i    QAbstractItemModel
    ---------------------------------
    child(r, c)      index(r, c, i)
    data()           data(i, ...)
    flags()          flags(i)
    parent()         parent(i)
    sibling(r, c)    sibling(r, c, i)

The QModelIndex values returned by a model are not persistent, i.e.
operations changing the model might invalidate existing QModelIndex
objects. There is QPersistentModelIndex that listens on change
signals of a QAbstractItemModel and updates its reference to an item
until it is removed from the model, itself.

## tree_model::Base comparison

The tree_model::Base abstract base class mirrors tree data structures where
looking up the rank of an element or the number of children is expensive.
Instead, the model provides methods to get the previous and next sibling of an
element (in addition to a method that returns a parent.  Consequently, there is
no method that returns the n-th child of an element.

In contrast, QAbstractItemModel is all about rank operations. It has methods
for returning the n-th child, the n-th sibling, the children count, etc. and
each generated QModelIndex must contain the rank of the element inside its
siblings.

Existing tree data-structures (e.g. a DOM tree) usually don't provide
efficient methods for such rank operations. Thus, extending QAbstractItemModel
to interface with such a tree data-structures is likely challenging.

## Item_Adaptor Ansatz

The Item_Adaptor class is part of a layered approach to implementing new
QAbstractItem based tree models. The Item_Adaptor layer just implements the
translation of rank operations (as seen in QAbstractItemModel) to non-rank ones
(as seen in tree_model::Base). It goes into some length to implement the rank
operations efficiently, i.e. it does some caching using hash tables and ranked
lists (a ranked list is a data structure where the n-th element can be looked
up efficiently). Also, it overrides the default implementation of
QAbstractItemModel::sibling() to enable the efficient iteration of all children
of an element.

Instead of having to implement the (hopefully efficient) translation of rank
operations for each existing tree data structure, one can instead extend just
tree_model::Base and re-use the rank translation code via instantiating
Item_Adaptor.

 */

namespace tree_model {

  Item_Adaptor::Item_Adaptor(Base *model, QObject *parent)
    :
      QAbstractItemModel(parent),
      model_(model)
  {
    model_->setParent(this);
    if (model_) {
      connect(model_, &Base::data_about_to_be_changed,
          this, &Item_Adaptor::emit_data_about_to_be_changed);
      connect(model_, &Base::data_changed,
          this, &Item_Adaptor::emit_data_changed);
      connect(model_, &Base::index_about_to_be_removed,
          this, &Item_Adaptor::call_begin_remove_row);
      connect(model_, &Base::index_removed,
          this, &Item_Adaptor::call_end_remove_row);

      connect(model_, &Base::index_about_to_be_inserted,
          this, &Item_Adaptor::call_begin_insert_row);
      connect(model_, &Base::index_inserted,
          this, &Item_Adaptor::call_end_insert_row);
    }
  }

  Index Item_Adaptor::create_index(const QModelIndex &i) const
  {
    Index r(
        model_->create_index(i.column() < 0 ?
          0 : i.column(), i.internalPointer()));
    return r;
  }

  QModelIndex Item_Adaptor::create_index(const Index &idx) const
  {
    if (idx.is_valid()) {
#ifdef USE_SLOW_TREE_MODEL
      Index p(idx.parent());
      int row = 0;
      for (Index i = model_->first_child(p); i.is_valid();
          i = model_->next_sibling(i), ++row)
        if (i.internal_pointer() == idx.internal_pointer())
          return createIndex(row, idx.column(), idx.internal_pointer());
      return QModelIndex();
#else
      Index p(idx.parent());
      auto &children = cached_children(p);
      auto node = index_to_node_map_.at(idx.internal_pointer());
      int row = children.rank(*node);
      return createIndex(row, idx.column(), idx.internal_pointer());
#endif
    } else {
      return QModelIndex();
    }
  }

  std::deque<void*> Item_Adaptor::ancestors(const Index &x,
      bool include_self) const
  {
    std::deque<void*> r;
    if (include_self)
      r.push_back(x.internal_pointer());
    Index i(x);
    do {
      i = i.parent();
      r.push_back(i.internal_pointer());
    } while (i.is_valid());
    return r;
  }

  list::ranked::List<void*> &Item_Adaptor::cached_children(
      const Index &p) const
  {
    auto that = const_cast<Item_Adaptor*>(this);
    auto x = that->index_to_children_map_.find(p.internal_pointer());
    if (x == that->index_to_children_map_.end()) {
      auto ancestors = this->ancestors(p, true);
      list::ranked::List<void*> l;
      for (Index i = model_->last_child(p); i.is_valid();
          i = model_->prev_sibling(i)) {
        l.push_front(i.internal_pointer());
        that->index_to_node_map_[i.internal_pointer()] = &l.node(0);
        for (auto a : ancestors)
          that->index_to_descendants_map_[a].insert(i.internal_pointer());
      }
      auto r = that->index_to_children_map_.insert(
          std::make_pair(p.internal_pointer(),
            std::move(l)));
      x = r.first;
    }
    return x->second;
  }

  QModelIndex Item_Adaptor::index(int row, int column,
      const QModelIndex &parent) const
  {
    Index p(create_index(parent));
    if (column >= int(model_->column_count(p)))
      return QModelIndex();

#ifdef USE_SLOW_TREE_MODEL
    int r = 0;
    for (Index i = model_->first_child(p); i.is_valid();
        //i = model_->next_sibling(i), ++r) {
        i = i.next_sibling(), ++r) {
      if (r == row)
        return createIndex(row, column, i.internal_pointer());
    }
#else
    if (row >= 0) {
      auto &children = cached_children(p);
      if (size_t(row) < children.size())
        return createIndex(row, column, children[row]);
    }
#endif

    return QModelIndex();
  }

  QModelIndex Item_Adaptor::sibling(int row, int column,
      const QModelIndex &index) const
  {
    QModelIndex parent(this->parent(index));
    if (column >= columnCount(parent))
      return QModelIndex();
    if (!index.isValid())
      return this->index(row, column, parent);
    Index i(model_->create_index(0, index.internalPointer()));
    if (index.row() + 1 == row) {
      i = i.next_sibling();
      if (i.is_valid())
        return createIndex(row, column, i.internal_pointer());
    } else if (index.row() == row + 1) {
      i = i.prev_sibling();
      if (i.is_valid())
        return createIndex(row, column, i.internal_pointer());
    } else if (index.row() == row) {
      if (i.is_valid())
        return createIndex(row, column, i.internal_pointer());
#ifdef USE_SLOW_TREE_MODEL
    } else if (index.row() < row) {
      int r = index.row() + 2;
      i = i.next_sibling();
      i = i.next_sibling();
      for (; i.is_valid(); i = model_->next_sibling(i), ++r)
        if (r == row)
          return createIndex(row, column, i.internal_pointer());
    } else {
      int r = index.row() - 2;
      i = i.prev_sibling();
      i = i.prev_sibling();
      for (; i.is_valid(); i = model_->prev_sibling(i), --r)
        if (r == row)
          return createIndex(row, column, i.internal_pointer());
    }
#else
    } else
      return this->index(row, column, parent);
#endif
    return QModelIndex();
  }

  QModelIndex Item_Adaptor::parent(const QModelIndex &child) const
  {
    Index c(create_index(child));
    Index p(c.parent());
    return create_index(p);
  }

  bool Item_Adaptor::hasChildren(const QModelIndex &parent) const
  {
    Index p(create_index(parent));
    return model_->has_children(p);
  }

  int Item_Adaptor::rowCount(const QModelIndex &parent) const
  {
    Index p(create_index(parent));
    if (model_->has_children(p)) {
#ifdef USE_SLOW_TREE_MODEL
      int n = 0;
      for (Index i = model_->first_child(p); i.is_valid();
          i = model_->next_sibling(i))
        ++n;
      return n;
#else
      auto &children = cached_children(p);
      return children.size();
#endif
    } else {
      return 0;
    }
  }

  int Item_Adaptor::columnCount(const QModelIndex &parent) const
  {
    Index p(create_index(parent));
    unsigned r = model_->column_count(p);
    return r;
  }

  QVariant Item_Adaptor::data(const QModelIndex &index, int role) const
  {
    if (role == Qt::ToolTipRole && display_rank_) {
      if (index.isValid())
        return QVariant(QString("#%1").arg(index.row()+1));
      else
        return QVariant();
    } else {
      Index i(create_index(index));
      return model_->data(i, role);
    }
  }

  bool Item_Adaptor::setData(const QModelIndex &index, const QVariant &value,
      int role)
  {
    Index i(create_index(index));
    bool r = model_->set_data(i, value, role);
    // we don't directly emit those signals here, instead
    // we connect to the slots of the wrapped model and forward them
    //if (r)
    //  emit dataChanged(index, index);
    return r;
  }

  QVariant Item_Adaptor::headerData(int section, Qt::Orientation orientation,
      int role) const
  {
    if (orientation == Qt::Horizontal)
      return model_->header_data(section, orientation, role);
    else
      return QVariant();
  }

  Qt::ItemFlags Item_Adaptor::flags(const QModelIndex &index) const
  {
    Index i(create_index(index));
    return model_->flags(i);
  }

  void Item_Adaptor::emit_data_about_to_be_changed(const Index &index,
      const QVector<int> &roles)
  {
    QModelIndex i(create_index(index));
    emit dataAboutToBeChanged(i, i, roles);
  }

  void Item_Adaptor::emit_data_changed(const Index &index,
      const QVector<int> &roles)
  {
    QModelIndex i(create_index(index));
    emit dataChanged(i, i, roles);
  }

  bool Item_Adaptor::removeRows(int row, int count,
      const QModelIndex &parent)
  {
    bool r = false;
    std::deque<Index> ys;
    remove_row_ = row;
    for (int i = row; i < row+count; ++i) {
      QModelIndex x(index(i, 0, parent));
      ys.push_back(create_index(x));
    }
    for (auto &y : ys) {
      bool b = model_->remove(y);
      r = r || b;
    }
    return r;
  }

  void Item_Adaptor::call_begin_remove_row(const Index &index)
  {
    QModelIndex i(create_index(index));
    beginRemoveRows(i.parent(), i.row(), i.row());

#ifndef USE_SLOW_TREE_MODEL
    if (index.is_valid()) {
      auto ancestors = this->ancestors(index);
      index_to_children_map_.erase(index.internal_pointer());
      auto &children = cached_children(index.parent());
      children.remove(remove_row_);
      index_to_node_map_.erase(index.internal_pointer());
      for (auto a : ancestors)
        index_to_descendants_map_[a].erase(index.internal_pointer());
    }
#endif
  }

  void Item_Adaptor::call_end_remove_row(const Index &y)
  {
    endRemoveRows();
  }

  bool Item_Adaptor::insertRows(int row, int count,
      const QModelIndex &parent)
  {
    if (parent.column() > 0)
      return false;
    bool r = false;
    int row_count = rowCount(parent);
    insert_row_ = row;
    if (row == row_count) {
      Index p(create_index(parent));
      for (int k = 0; k < count; ++k) {
        auto new_index = model_->insert(p, -1);
        r = r || new_index.is_valid();
        if (new_index.is_valid())
          ++insert_row_;
      }
    } else if (row < row_count) {
      for (int k = 0; k < count; ++k) {
        QModelIndex i(index(row, 0, parent));
        Index j(create_index(i));
        auto new_index = model_->insert(j, -2);
        r = r || new_index.is_valid();
        if (new_index.is_valid())
          ++insert_row_;
      }
    }
    return r;
  }

  void Item_Adaptor::call_begin_insert_row(const Index &index, int position)
  {
    QModelIndex i(create_index(index));
    switch (position) {
      case 1: // first child
        beginInsertRows(i, 0, 0);
        break;
      case -1: // last child
        {
        int row_count = rowCount(i);
        beginInsertRows(i, row_count, row_count);
        }
        break;
      case -2: // before node
        {
        beginInsertRows(i.parent(), i.row(), i.row());
        }
        break;
      case 2: // after node
        {
        beginInsertRows(i.parent(), i.row()+1, i.row()+1);
        }
        break;
    }
  }

  void Item_Adaptor::call_end_insert_row(const Index &i)
  {
#ifndef USE_SLOW_TREE_MODEL
    if (i.is_valid()) {
      auto &children = cached_children(i.parent());
      auto &node = children.insert(insert_row_, i.internal_pointer());
      index_to_node_map_[i.internal_pointer()] = &node;
      auto ancestors = this->ancestors(i);
      for (auto a : ancestors)
        index_to_descendants_map_[a].insert(i.internal_pointer());
    }
#endif
    endInsertRows();
  }

  QStringList Item_Adaptor::mimeTypes() const
  {
    return model_->mime_types();
  }

  QMimeData *Item_Adaptor::mimeData(const QModelIndexList &indexes) const
  {
    std::deque<Index> is;
    for (auto &i : indexes)
      is.push_back(create_index(i));
    auto up = model_->mime_data(is);
    return up.release();
  }

  bool Item_Adaptor::dropMimeData(const QMimeData *data, Qt::DropAction action,
      int row, int column, const QModelIndex &parent)
  {
    if (row == -1 && column == -1) {
      insert_row_ = rowCount(parent);
      return model_->drop_mime_data(data, action, create_index(parent), -1);
    } else {
      insert_row_ = row;
      return model_->drop_mime_data(data, action,
          create_index(index(row, column, parent)), -2);
    }
  }

}


