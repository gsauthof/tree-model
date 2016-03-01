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
#ifndef TREE_MODEL_ITEM_ADAPTOR_HH
#define TREE_MODEL_ITEM_ADAPTOR_HH

#include <QAbstractItemModel>

#include <unordered_map>
#include <unordered_set>
#include <deque>

#include <list/ranked_list.hh>

#include "index.hh"

namespace tree_model {

  class Base;


  class Item_Adaptor : public QAbstractItemModel
  {
    Q_OBJECT
    public:
      explicit Item_Adaptor(Base *model, QObject *parent = nullptr);

      QModelIndex index(int row, int column,
          const QModelIndex &parent = QModelIndex()) const override;
      QModelIndex sibling(int row, int column, const QModelIndex & index)
        const override;


      QModelIndex parent(const QModelIndex &child) const override;

      bool hasChildren(const QModelIndex &parent) const override;
      int rowCount(const QModelIndex &parent) const override;
      int columnCount(const QModelIndex &parent) const override;

      QVariant data(const QModelIndex &index,
          int role = Qt::DisplayRole) const override;
      bool setData(const QModelIndex &index, const QVariant &value,
          int role = Qt::EditRole) override;

      QVariant headerData(int section, Qt::Orientation orientation,
          int role = Qt::DisplayRole) const override;

      Qt::ItemFlags flags(const QModelIndex &index) const override;

      bool removeRows(int row, int count,
          const QModelIndex &parent = QModelIndex()) override;

      bool insertRows(int row, int count,
          const QModelIndex &parent = QModelIndex()) override;

      QStringList mimeTypes() const override;
      QMimeData *mimeData(const QModelIndexList &indexes) const override;
      bool dropMimeData(const QMimeData *data, Qt::DropAction action,
          int row, int column, const QModelIndex &parent) override;
      Qt::DropActions supportedDropActions() const override;
      bool canDropMimeData(const QMimeData *data, Qt::DropAction action,
          int row, int column, const QModelIndex &parent) const override;

    signals:
      void dataAboutToBeChanged(const QModelIndex &topLeft,
          const QModelIndex &bottomRight,
          const QVector<int> &roles = QVector<int>());

    public slots:

    private slots:
      void emit_data_about_to_be_changed(const Index &index,
          const QVector<int> &roles = QVector<int>());
      void emit_data_changed(const Index &index,
          const QVector<int> &roles = QVector<int>());

      void call_begin_remove_row(const Index &index);
      void call_end_remove_row(const Index &index);

      void call_begin_insert_row(const Index &index, int position);
      void call_end_insert_row(const Index&);
    protected:
      Index create_index(const QModelIndex &i) const;
      QModelIndex create_index(const Index &idx) const;
      Base *model_ {nullptr};
    private:
      bool display_rank_ {true};
      bool ignore_neutral_set_data_ {true};

      std::unordered_map<void*, list::ranked::List<void*>::Basic_Node*>
        index_to_node_map_;
      std::unordered_map<void*, list::ranked::List<void*> >
        index_to_children_map_;
      std::unordered_map<void*, std::unordered_set<void*> >
        index_to_descendants_map_;

      size_t insert_row_ {0};
      size_t remove_row_ {0};

      list::ranked::List<void*> &cached_children(const Index &p) const;
      std::deque<void*> ancestors(const Index &x, bool include_self = true)
        const;

      std::pair<Index, int> drop_location(int row, int column,
          const QModelIndex &parent) const;

  };

}

#endif
