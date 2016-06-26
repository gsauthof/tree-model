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
#ifndef TREE_MODEL_PROTECTED_ITEM_MODEL_HH
#define TREE_MODEL_PROTECTED_ITEM_MODEL_HH

#include <Qt>
#include <QModelIndex>

class QVariant;
class QMimeData;

namespace tree_model {

  template <typename T>
  class Protected_Item_Model : public T {
    public:
      using T::T;

      bool setData(const QModelIndex &index, const QVariant &value,
          int role = Qt::EditRole) override;
      Qt::ItemFlags flags(const QModelIndex &index) const override;

      bool removeRows(int row, int count,
          const QModelIndex &parent = QModelIndex()) override;
      bool insertRows(int row, int count,
          const QModelIndex &parent = QModelIndex()) override;
      bool dropMimeData(const QMimeData *data, Qt::DropAction action,
          int row, int column, const QModelIndex &parent) override;
      bool canDropMimeData(const QMimeData *data, Qt::DropAction action,
          int row, int column, const QModelIndex &parent) const override;

      void set_read_only(bool b);
    private:
      bool read_only_ {false};
  };

} // tree_model

#endif // TREE_MODEL_PROTECTED_ITEM_MODEL_HH
