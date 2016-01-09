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
#include "insert_rows.hh"

#include <QModelIndex>
#include <QAbstractItemModel>
#include <QDebug>

using namespace std;

namespace tree_model {
  namespace operation {

    Insert_Rows::Insert_Rows(const QModelIndex &parent, int first, int last)
      :
        parent_(parent),
        first_(first),
        count_(last-first+1)
    {
      begin_finalize();
    }
    void Insert_Rows::forward(QAbstractItemModel &model)
    {
      begin_forward();
      model.insertRows(first_, count_, parent_);
    }
    void Insert_Rows::rewind(QAbstractItemModel &model)
    {
      begin_rewind();
      model.removeRows(first_, count_, parent_);
    }

  }
}
