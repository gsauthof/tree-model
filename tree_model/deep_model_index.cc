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
#include "deep_model_index.hh"

#include <QModelIndex>

using namespace std;

namespace tree_model {

  Deep_Model_Index::Deep_Model_Index() =default;
  Deep_Model_Index::Deep_Model_Index(const QModelIndex &o)
  {
    if (o.isValid()) {
      model_ = o.model();
      column_ = o.column();
      for (QModelIndex i(o.sibling(o.row(), 0)); i.isValid(); i = i.parent())
         row_stack_.push_front(i.row());
    }
  }

  Deep_Model_Index::operator QModelIndex () const
  {
    if (column_ == -1 || !model_)
      return QModelIndex();
    else {
      QModelIndex p;
      for (auto row : row_stack_)
        p = model_->index(row, 0, p);
      return p.sibling(p.row(), column_);
    }
  }

  bool Deep_Model_Index::is_valid() const
  {
    return column_ != -1;
  }

}
