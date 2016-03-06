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
#ifndef EDITOR_GUI_COMMAND_CLIPBOARD_PASTE_HH
#define EDITOR_GUI_COMMAND_CLIPBOARD_PASTE_HH

#include <QObject>


class QAbstractItemModel;
class QItemSelectionModel;

#include <QModelIndex>

namespace editor {
  namespace gui_command {

    class Clipboard_Paste : public QObject {
      Q_OBJECT
      public:
        using QObject::QObject;

      public slots:
        void set_model(QAbstractItemModel *model);

        void paste(const QModelIndexList &is);
        void paste_as_child(const QModelIndexList &is);

      protected:
              QAbstractItemModel  *model_  {nullptr};
      private:
        void paste(const QModelIndexList &is, bool insert_before);
    };

  } // namespace gui_command
} // namespace editor

#endif
