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

#ifndef EDITOR_COMMAND_NEW_HH
#define EDITOR_COMMAND_NEW_HH

#include <QObject>

namespace tree_model {
  class Base;
}
class QAbstractItemModel;

namespace editor {
  namespace command {

    class New : public QObject {
      Q_OBJECT
      public:
        using QObject::QObject;

        void create();

      signals:
        void item_tree_model_created(QAbstractItemModel *model);
        void tree_model_created(tree_model::Base *model);

    };
  } // command
} //editor

#endif
