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
#ifndef TREE_MODEL_DEEP_MODEL_INDEX_HH
#define TREE_MODEL_DEEP_MODEL_INDEX_HH

#include <deque>

class QAbstractItemModel;
class QModelIndex;


/* Converts a non-persitent QModelIndex into a persistent one.

Main difference to [QPersistentModelIndex][qpers] is that
Deep_Model_Index also allows for referenced elements being
temporarily removed. When structure changing operations after
and including the removal are rolled back, the Deep_Model_Index
is still valid.

Also it does not connects to any signals of the model
referenced by the [QModelIndex][qmodi].

[qpers]: http://doc.qt.io/qt-5/qpersistentmodelindex.html
[qmodi]: http://doc.qt.io/qt-5/qmodelindex.html

*/

namespace tree_model {

  class Deep_Model_Index {
    private:
      const QAbstractItemModel *model_ {nullptr};
      std::deque<int> row_stack_;
      int column_ {-1};
    public:
      Deep_Model_Index();
      Deep_Model_Index(const QModelIndex &o);

      operator QModelIndex () const;

      bool is_valid() const;
  };

}

#endif
