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
#ifndef MAIN_WINDOW_HH
#define MAIN_WINDOW_HH

#include <QMainWindow>

class QModelIndex;

namespace editor {
  class Recent_Menu;
  class Tree_Widget;

  namespace Ui {
    class Main_Window;
  }

  class Main_Window : public QMainWindow
  {
    Q_OBJECT

    public:
      explicit Main_Window(QWidget *parent = nullptr);
      ~Main_Window();
      Recent_Menu &recent_menu();
      Tree_Widget &tree_widget();
      QAction &new_action();
      QAction &open_action();
      QAction &save_action();
      QAction &save_as_action();
      QAction &save_a_copy_action();
      QAction &undo_action();
      QAction &redo_action();
      QAction &cut_action();
      QAction &copy_action();
      QAction &paste_action();
      QAction &paste_as_child_action();
      QAction &display_subtree_action();
      QAction &quit_action();

    public slots:
      void display_status(const QString &msg);
      void update_window_title(const QString &filename);

    signals:
      void add_child_triggered  (const QModelIndex &);
      void add_sibling_triggered(const QModelIndex &);

    protected:
      void closeEvent(QCloseEvent *event) override;
    private slots:
    private:
      Ui::Main_Window *ui {nullptr};
      Recent_Menu *recent_menu_ {nullptr};

      void setup_shortcuts();
  };

}

#endif // MAIN_WINDOW_HH
