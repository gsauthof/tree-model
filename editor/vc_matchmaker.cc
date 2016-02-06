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
#include "vc_matchmaker.hh"

#include "gui_controller.hh"
#include "main_window.hh"
#include "recent_menu.hh"
#include "tree_widget.hh"
#include "tree_view.hh"
#include "subtree_window.hh"

namespace editor {

  static void connect_action_triggers(
      Main_Window &w, Gui_Controller &c)
  {
    w.connect(&w.open_action(), &QAction::triggered,
       &c, &Gui_Controller::select_open);
    w.connect(&w.save_action(), &QAction::triggered,
        &c, &Gui_Controller::save);
    w.connect(&w.save_as_action(), &QAction::triggered,
        &c, &Gui_Controller::select_save);
    w.connect(&w.save_a_copy_action(), &QAction::triggered,
        &c, &Gui_Controller::select_save_copy);
    w.connect(&w.undo_action(), &QAction::triggered,
        &c, &Controller::undo);
    w.connect(&w.redo_action(), &QAction::triggered,
        &c, &Controller::redo);
    w.connect(&w.copy_action(), &QAction::triggered,
        &c, &Gui_Controller::clipboard_copy);
    w.connect(&w.cut_action(), &QAction::triggered,
        &c, &Gui_Controller::clipboard_cut);
    w.connect(&w.paste_action(), &QAction::triggered,
        &c, &Gui_Controller::clipboard_paste);
    w.connect(&w.paste_as_child_action(), &QAction::triggered,
        &c, &Gui_Controller::clipboard_paste_as_child);
    w.connect(&w.display_subtree_action(), &QAction::triggered,
        &c, &Gui_Controller::display_subtree);
  }

  static void connect_enable_signals(
      Main_Window &w, Gui_Controller &c)
  {
    c.connect(&c, &Controller::rewind_enabled,
        &w.undo_action(), &QAction::setEnabled);
    c.connect(&c, &Controller::forward_enabled,
        &w.redo_action(), &QAction::setEnabled);
  }

  static void connect_recent_menu(
      Recent_Menu &m, Gui_Controller &c)
  {
    c.connect(&c, &Controller::file_opened,
        &m, &Recent_Menu::register_file_open);
    m.connect(&m, &Recent_Menu::chosen,
        &c, &Gui_Controller::open);
  }

  static void connect_main_window(
      Main_Window &w, Gui_Controller &c)
  {
    connect_action_triggers(w, c);
    connect_enable_signals(w, c);
    connect_recent_menu(w.recent_menu(), c);

    // updates the window title iff it contains the [*] placeholder
    c.connect(&c, &Controller::model_changed,
        &w, &QMainWindow::setWindowModified);

    c.connect(&c, &Gui_Controller::msg_produced,
        &w, &Main_Window::display_status);

    c.connect(&c, &Controller::file_opened,
        &w, &Main_Window::update_window_title);
    c.connect(&c, &Controller::saved,
        &w, &Main_Window::update_window_title);
  }

  static void connect_tree_widget(Tree_Widget &w, Gui_Controller &c)
  {
    Tree_View &tv = w.tree_view();

    c.connect(&c, &Gui_Controller::item_tree_model_created,
        &tv, &Tree_View::set_model);

    tv.connect(&tv, &editor::Tree_View::context_requested,
        &c, &Gui_Controller::display_tree_context);
    tv.connect(&tv, &editor::Tree_View::remove_triggered,
        &c, &Controller::remove);

    tv.connect(&tv, &editor::Tree_View::selection_model_changed,
        &c, &Gui_Controller::selection_model_changed);
  }

  static void connect_sub_tree_widgets(Gui_Controller &c)
  {
    c.connect(&c, &Gui_Controller::subtree_window_created,
        [&c](auto w){
        c.connect(&c, &Controller::item_tree_model_created,
            &w->tree_widget().tree_view(), &Tree_View::set_model);
        });
  }

  void connect_view_controller(
      Main_Window &w, Gui_Controller &c)
  {
    connect_main_window(w, c);
    connect_tree_widget(w.tree_widget(), c);
    connect_sub_tree_widgets(c);
  }

}
