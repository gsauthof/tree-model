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

#include <QUrl>

namespace editor {

  static void connect_action_triggers(
      Main_Window &w, Gui_Controller &c)
  {
    w.connect(&w, &Main_Window::open_triggered,
        &c, &Gui_Controller::select_open);
    w.connect(&w, &Main_Window::open_urls_requested,
        &c, &Gui_Controller::open_urls);
    w.connect(&w, &Main_Window::save_triggered,
        &c, &Gui_Controller::save);
    w.connect(&w, &Main_Window::save_as_triggered,
        &c, &Gui_Controller::select_save);
    w.connect(&w, &Main_Window::save_a_copy_triggered,
        &c, &Gui_Controller::select_save_copy);
    w.connect(&w, &Main_Window::undo_triggered,
        &c, &Controller::undo);
    w.connect(&w, &Main_Window::redo_triggered,
        &c, &Controller::redo);
    w.connect(&w, &Main_Window::display_subtree_triggered,
        &c, &Gui_Controller::display_subtree);
    w.connect(&w, &Main_Window::write_aci_triggered,
        &c, &Gui_Controller::write_aci);
  }

  static void connect_enable_signals(
      Main_Window &w, Gui_Controller &c)
  {
    c.connect(&c, &Controller::rewind_enabled,
        &w, &Main_Window::enable_undo);
    c.connect(&c, &Controller::forward_enabled,
        &w, &Main_Window::enable_redo);
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

  static void connect_tree_view(Tree_View &tv, Gui_Controller &c)
  {
    c.connect(&c, &Gui_Controller::item_tree_model_created,
        &tv, &Tree_View::set_model);

    tv.connect(&tv, &editor::Tree_View::remove_triggered,
        &c, &Controller::remove);
    tv.connect(&tv, &editor::Tree_View::edit_triggered,
        &c, &Gui_Controller::edit);
    tv.connect(&tv, &editor::Tree_View::add_child_triggered,
        &c, &Gui_Controller::add_child);
    tv.connect(&tv, &editor::Tree_View::add_sibling_triggered,
        &c, &Gui_Controller::add_sibling);
    tv.connect(&tv, &editor::Tree_View::cut_triggered,
        &c, &Gui_Controller::clipboard_cut);
    tv.connect(&tv, &editor::Tree_View::copy_triggered,
        &c, &Gui_Controller::clipboard_copy);
    tv.connect(&tv, &editor::Tree_View::paste_triggered,
        &c, &Gui_Controller::clipboard_paste);
    tv.connect(&tv, &editor::Tree_View::paste_as_child_triggered,
        &c, &Gui_Controller::clipboard_paste_as_child);
  }

  static void connect_tree_widget(Tree_Widget &w, Gui_Controller &c)
  {
    Tree_View &tv = w.tree_view();
    connect_tree_view(tv, c);
    tv.connect(&tv, &editor::Tree_View::selection_model_changed,
        &c, &Gui_Controller::selection_model_changed);
  }

  static void connect_sub_tree_widgets(Gui_Controller &c)
  {
    c.connect(&c, &Gui_Controller::subtree_window_created,
        [&c](auto w){ connect_tree_view(w->tree_widget().tree_view(), c); });
  }

  void connect_view_controller(
      Main_Window &w, Gui_Controller &c)
  {
    connect_main_window(w, c);
    connect_tree_widget(w.tree_widget(), c);
    connect_sub_tree_widgets(c);
  }

}
