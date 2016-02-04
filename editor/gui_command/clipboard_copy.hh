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
#ifndef EDITOR_GUI_COMMAND_CLIPBOARD_COPY_HH
#define EDITOR_GUI_COMMAND_CLIPBOARD_COPY_HH

#include <QObject>


class QAbstractItemModel;
class QItemSelectionModel;

#include <QModelIndex>

namespace editor {
  namespace gui_command {

    class Clipboard_Copy : public QObject {
      Q_OBJECT
      public:
        explicit Clipboard_Copy(QObject *parent = nullptr);

      public slots:
        void set_model(const QAbstractItemModel *model);
        void set_selection_model(const QItemSelectionModel *smodel);

        void copy();

      protected:
        const QAbstractItemModel  *model_  {nullptr};
        const QItemSelectionModel *smodel_ {nullptr};

        QModelIndexList selected_indexes();
    };

  } // namespace gui_command
} // namespace editor

#endif
