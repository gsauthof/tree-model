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
#include "remove_rows.hh"

#include <QModelIndex>
#include <QAbstractItemModel>
#include <QMimeData>
#include <QDebug>

using namespace std;

namespace tree_model {
  namespace operation {

    Remove_Rows::Remove_Rows(
        const QModelIndex &parent, int first, int last,
        QAbstractItemModel &model
        )
      :
        parent_(parent),
        first_(first),
        last_(last),
        row_count_(model.rowCount(parent_))
    {
      begin_finalize();

      for (int i = first; i <= last; ++i) {
        data_.emplace_back(model.mimeData(QModelIndexList()
              << model.index(i, 0, parent)));
      }
    }

    Remove_Rows::~Remove_Rows() =default;

    void Remove_Rows::forward(QAbstractItemModel &model)
    {
      begin_forward();
      model.removeRows(first_, last_ - first_ + 1, parent_);
    }

    void Remove_Rows::rewind(QAbstractItemModel &model)
    {
      begin_rewind();
      QModelIndex parent(parent_);
      if (last_ + 1 == row_count_) {
        for (auto &d : data_)
          model.dropMimeData(d.get(), Qt::CopyAction, -1, -1, parent);
      } else {
        for (auto i = data_.rbegin(); i != data_.rend(); ++i)
          model.dropMimeData((*i).get(), Qt::CopyAction, first_, 0, parent);
      }
    }

  }
}
