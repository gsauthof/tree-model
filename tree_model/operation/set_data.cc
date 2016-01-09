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
#include "set_data.hh"

#include <QModelIndex>
#include <QAbstractItemModel>
#include <QDebug>

using namespace std;
namespace tree_model {
  namespace operation {

    Set_Data::Set_Data(const QModelIndex &top_left,
        const QModelIndex &bottom_right,
        QAbstractItemModel &model,
        const QVector<int> &roles
        )
      :
        top_left_(top_left),
        bottom_right_(bottom_right),
        roles_(roles),
        old_data_(get_data(top_left, bottom_right, model))
    {
    }

    void Set_Data::update(const QModelIndex &top_left,
        const QModelIndex &bottom_right,
        QAbstractItemModel &model,
        const QVector<int> &roles
        )
    {
      begin_finalize();
      new_data_ = get_data(top_left, bottom_right, model);
    }

    void Set_Data::forward(QAbstractItemModel &model)
    {
      begin_forward();
      set_data(new_data_, model);
    }
    void Set_Data::rewind(QAbstractItemModel &model)
    {
      begin_rewind();
      set_data(old_data_, model);
    }
    void Set_Data::set_data(const std::deque<QMap<int, QVariant> > &data,
        QAbstractItemModel &model) const
    {
      QModelIndex top_left(top_left_);
      QModelIndex bottom_right(bottom_right_);
      if (top_left.parent() != bottom_right.parent())
        throw logic_error("Set_Data: must have same parents");
      int m = top_left.row() - bottom_right.row() + 1;
      int n = top_left.column() - bottom_right.column() + 1;
      if (m*n != int(data.size()))
        throw logic_error("Set_Data: not enough data points recorded");
      auto k = data.begin();
      for (int i = top_left.row(); i<=bottom_right.row(); ++i) {
        for (int j = top_left.column(); j<=bottom_right.column(); ++j) {
          bool r = model.setItemData(top_left.sibling(i, j), *k);
          (void)r;
          ++k;
        }
      }
    }
    std::deque<QMap<int, QVariant> > Set_Data::get_data(
        const QModelIndex &top_left, const QModelIndex &bottom_right,
        QAbstractItemModel &model)
    {
      deque<QMap<int, QVariant> > data;
      for (int i = top_left.row(); i<=bottom_right.row(); ++i) {
        for (int j = top_left.column(); j<=bottom_right.column(); ++j) {
          //if (roles_.empty()) {
          //  data.push_back(model.itemData(top_left.sibling(i, j)));
          //} else {
            QMap<int, QVariant> m;
            for (auto r : roles_) {
              m[r] = model.data(top_left.sibling(i, j), r);
            }
            data.push_back(std::move(m));
          //}
        }
      }
      return data;
    }

  }
}
