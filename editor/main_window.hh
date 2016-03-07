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
#include <QList>
#include <QUrl>

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

    public slots:
      void display_status(const QString &msg);
      void update_window_title(const QString &filename);
      void enable_undo(bool b);
      void enable_redo(bool b);

    signals:
      void new_triggered();
      void open_triggered();
      void save_triggered();
      void save_as_triggered();
      void save_a_copy_triggered();
      void undo_triggered();
      void redo_triggered();
      void display_subtree_triggered();
      void quit_triggered();
      void open_urls_requested(const QList<QUrl> &urls);


    protected:
      void closeEvent(QCloseEvent *event) override;

      void dragEnterEvent(QDragEnterEvent *event) override;
      //void dragLeaveEvent(QDragLeaveEvent *event) override;
      void dragMoveEvent(QDragMoveEvent *event) override;
      void dropEvent(QDropEvent *event) override;
    private slots:
    private:
      Ui::Main_Window *ui {nullptr};
      Recent_Menu *recent_menu_ {nullptr};

      void restore_settings();
      void store_settings();
      void setup_shortcuts();
      void setup_icons();
      void setup_about_action();
      void setup_clear_action();
      void connect_actions_to_tree_view();
      void setup_selection_dependent_actions();
      void forward_trigger_signals();
  };

}

#endif // MAIN_WINDOW_HH
