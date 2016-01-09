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
#ifndef TREE_MODEL_OPERATION_SET_DATA_HH
#define TREE_MODEL_OPERATION_SET_DATA_HH

#include <deque>
#include <QVariant>
#include <QVector>
#include <QMap>
#include <tree_model/operation/base.hh>
#include <tree_model/deep_model_index.hh>

class QAbstractItemModel;
class QModelIndex;

namespace tree_model {
  namespace operation {

    class Set_Data : public Base {
      private:
        Deep_Model_Index top_left_;
        Deep_Model_Index bottom_right_;
        QVector<int> roles_;
        std::deque<QMap<int, QVariant> > old_data_;
        std::deque<QMap<int, QVariant> > new_data_;
      public:
        Set_Data(const QModelIndex &top_left,
            const QModelIndex &bottom_right,
            QAbstractItemModel &model,
            const QVector<int> &roles = QVector<int>() << Qt::EditRole
            );

        void update(const QModelIndex &top_left,
            const QModelIndex &bottom_right,
            QAbstractItemModel &model,
            const QVector<int> &roles = QVector<int>() << Qt::EditRole
            );

        void forward(QAbstractItemModel &model) override;
        void rewind(QAbstractItemModel &model) override;

      private:
        std::deque<QMap<int, QVariant> > get_data(
            const QModelIndex &top_left, const QModelIndex &bottom_right,
            QAbstractItemModel &model);
        void set_data(const std::deque<QMap<int, QVariant> > &data,
            QAbstractItemModel &model) const;

    };

  }
}

#endif
