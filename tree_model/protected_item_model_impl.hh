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

#ifndef TREE_MODEL_PROTECTED_ITEM_MODEL_IMPL_HH
#define TREE_MODEL_PROTECTED_ITEM_MODEL_IMPL_HH

#include "protected_item_model.hh"

namespace tree_model {

  template <typename T>
  bool Protected_Item_Model<T>::setData(const QModelIndex &index,
      const QVariant &value, int role)
  {
    if (read_only_)
      return false;
    else
      return T::setData(index, value, role);
  }
  template <typename T>
  Qt::ItemFlags Protected_Item_Model<T>::flags(const QModelIndex &index) const
  {
    if (read_only_) {
      return T::flags(index) & (~Qt::ItemFlags(Qt::ItemIsEditable));
    } else
      return T::flags(index);
  }

  template <typename T>
  bool Protected_Item_Model<T>::removeRows(int row, int count,
      const QModelIndex &parent)
  {
    if (read_only_)
      return false;
    else
      return T::removeRows(row, count, parent);
  }

  template <typename T>
  bool Protected_Item_Model<T>::insertRows(int row, int count,
      const QModelIndex &parent)
  {
    if (read_only_)
      return false;
    else
      return T::insertRows(row, count, parent);
  }

  template <typename T>
  bool Protected_Item_Model<T>::dropMimeData(const QMimeData *data,
      Qt::DropAction action,
      int row, int column, const QModelIndex &parent)
  {
    if (read_only_)
      return false;
    else
      return T::dropMimeData(data, action, row, column, parent);
  }

  template <typename T>
  bool Protected_Item_Model<T>::canDropMimeData(const QMimeData *data,
      Qt::DropAction action,
      int row, int column, const QModelIndex &parent) const
  {
    if (read_only_)
      return false;
    else
      return T::canDropMimeData(data, action, row, column, parent);
  }

  template <typename T>
  void Protected_Item_Model<T>::set_read_only(bool b)
  {
    read_only_ = b;
  }


} // tree_model

#endif // TREE_MODEL_PROTECTED_ITEM_MODEL_IMPL_HH
