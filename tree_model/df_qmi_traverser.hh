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
#ifndef TREE_MODEL_DF_QMI_TRAVERSER_HH
#define TREE_MODEL_DF_QMI_TRAVERSER_HH

#include <QModelIndex>

namespace tree_model {

  class DF_QMI_Traverser {
    private:
      QModelIndex node_;
      size_t height_ {0};
    public:
      DF_QMI_Traverser(const QModelIndex &root);
      const QModelIndex &operator*() const;
      void advance();
      void skip_children();
      size_t height() const;
      bool eot() const;
  };

} // tree_model

#endif
