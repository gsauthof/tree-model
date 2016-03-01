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
#include "base.hh"

#include <stdexcept>

#include <QMimeData>

namespace tree_model {

  Base::Base(QObject *parent) : QObject(parent)
  {

  }

  void Base::save(const QString &filename)
  {
    throw std::logic_error("tree_model::Base::save() not implemented");
  }

  Qt::ItemFlags Base::flags(const Index &index) const
  {
    if (index.is_valid())
      return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    else
      return 0;
  }

  bool Base::remove(const Index &index)
  {
    return false;
  }


  Index Base::create_index(unsigned column, void *ptr) const
  {
    return Index(this, column, ptr);
  }

  bool Base::set_data(const Index &index,
      const QVariant &value, int role)
  {
    return false;
  }

  Index Base::attribute(const Index &index, unsigned column) const
  {
    if (column >= column_count(index))
      return Index();
    return create_index(column, index.internal_pointer());
  }

  void Base::begin_remove_index(const Index &index)
  {
    emit index_about_to_be_removed(index);
  }

  void Base::end_remove_index(const Index &index)
  {
    emit index_removed(index);
  }

  Index Base::insert(const Index &index, int position)
  {
    return Index();
  }
  void Base::begin_insert_index(const tree_model::Index &index, int position)
  {
    emit index_about_to_be_inserted(index, position);
  }
  void Base::end_insert_index(const tree_model::Index &index)
  {
    emit index_inserted(index);
  }

  QStringList Base::mime_types() const
  {
    return QStringList();
  }
  std::unique_ptr<QMimeData> Base::mime_data(
      const std::deque<Index> &indexes) const
  {
    return std::unique_ptr<QMimeData>();
  }
  bool Base::drop_mime_data(const QMimeData *data, Qt::DropAction action,
      const Index &index, int position)
  {
    return false;
  }
  Qt::DropActions Base::supported_drop_actions() const
  {
    // Qt::MoveAction
    return Qt::CopyAction;
  }
  bool Base::can_drop_mime_data(const QMimeData *data, Qt::DropAction action,
      const Index &index, int position) const
  {
    auto m = mime_types();
    return data
      && std::any_of(m.begin(), m.end(),
        [data](auto &f){ return data->hasFormat(f); })
      && (action & supported_drop_actions());
  }

  QVariant Base::header_data(size_t section, Qt::Orientation orientation,
      int role) const
  {
    if (role == Qt::DisplayRole)
      return QString("%1").arg(section);
    else
      return QVariant();
  }

}
