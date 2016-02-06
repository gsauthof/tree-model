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
#ifndef EDITOR_GUI_COMMAND_DISPLAY_SUBTREE_HH
#define EDITOR_GUI_COMMAND_DISPLAY_SUBTREE_HH

#include <QObject>


class QAbstractItemModel;
class QItemSelectionModel;
class QWidget;


namespace editor {
  class Subtree_Window;

  namespace gui_command {

    class Display_Subtree : public QObject {
      Q_OBJECT
      public:
        explicit Display_Subtree(QWidget *parent_widget = nullptr);

      public slots:
        void set_model(QAbstractItemModel *model);
        void set_selection_model(const QItemSelectionModel *smodel);

        void display();

      signals:
        void subtree_window_created(editor::Subtree_Window *w);

      protected:
        QWidget *parent_widget_ {nullptr};
              QAbstractItemModel  *model_  {nullptr};
        const QItemSelectionModel *smodel_ {nullptr};

    };

  } // namespace gui_command
} // namespace editor

#endif
