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
#ifndef EDITOR_GUI_COMMAND_SEARCH_HH
#define EDITOR_GUI_COMMAND_SEARCH_HH

#include <editor/command/async_search.hh>

class QWidget;
class QAbstractItemModel;

namespace editor {
  class Result_Window;

  namespace gui_command {

    class Search : public editor::command::Async_Search {
      Q_OBJECT
      public:
        explicit Search(QWidget *parent_widget = nullptr);

      public slots:
        void display();
        void set_model(QAbstractItemModel *model);

      signals:
        void msg_produced(const QString &msg);
        void result_window_created(editor::Result_Window *w);
        void current_changed(const QModelIndex &current,
            const QModelIndex &previous);

      protected:
        QWidget *parent_widget_ {nullptr};
        QAbstractItemModel *model_ {nullptr};
      private slots:
        void translate_current(const QModelIndex &i, const QModelIndex &j);
    };

  } // gui_command
} // editor

#endif // EDITOR_GUI_COMMAND_SEARCH_HH
