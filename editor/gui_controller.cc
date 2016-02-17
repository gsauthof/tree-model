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
#include "gui_controller.hh"

#include <QWidget>
#include <QItemSelectionModel>

#include <editor/command/remove.hh>
#include <editor/gui_command/open.hh>
#include <editor/gui_command/select_open.hh>
#include <editor/gui_command/save.hh>
#include <editor/gui_command/edit.hh>
#include <editor/gui_command/add.hh>
#include <editor/gui_command/clipboard_copy.hh>
#include <editor/gui_command/clipboard_cut.hh>
#include <editor/gui_command/clipboard_paste.hh>
#include <editor/gui_command/display_subtree.hh>
#include <editor/subtree_window.hh>
#include <tree_model/base.hh>

namespace editor {

  Gui_Controller::Gui_Controller(QWidget *parent)
    :
      Controller(static_cast<QObject*>(parent)),
      parent_widget_(parent),
      open_           (new gui_command::Open(parent_widget_)),
      select_open_    (new gui_command::Select_Open(parent_widget_)),
      save_           (new gui_command::Save(parent_widget_)),
      edit_           (new gui_command::Edit(parent_widget_)),
      add_            (new gui_command::Add(parent_widget_)),
      clipboard_copy_ (new gui_command::Clipboard_Copy(this)),
      clipboard_cut_  (new gui_command::Clipboard_Cut(this)),
      clipboard_paste_(new gui_command::Clipboard_Paste(this)),
      display_subtree_(new gui_command::Display_Subtree(parent_widget_))
  {
    connect(open_, &gui_command::Open::item_tree_model_created,
            this, &Gui_Controller::item_tree_model_created);
    connect(open_, &gui_command::Open::tree_model_created,
            this, &Gui_Controller::tree_model_created);
    connect(open_, &gui_command::Open::file_opened,
            this, &Gui_Controller::file_opened);
    connect(open_, &gui_command::Open::msg_produced,
            this, &Gui_Controller::msg_produced);

    connect(select_open_,
        &gui_command::Select_Open::item_tree_model_created,
            this, &Gui_Controller::item_tree_model_created);
    connect(select_open_,
        &gui_command::Select_Open::tree_model_created,
            this, &Gui_Controller::tree_model_created);
    connect(select_open_, &gui_command::Select_Open::file_opened,
            this, &Gui_Controller::file_opened);
    connect(select_open_, &gui_command::Select_Open::msg_produced,
            this, &Gui_Controller::msg_produced);

    connect(save_, &gui_command::Save::msg_produced,
            this, &Gui_Controller::msg_produced);
    connect(save_, &gui_command::Save::saved,
        this, &Controller::saved);

    connect(this, &Controller::tree_model_created,
            save_, &gui_command::Save::set_tree_model);
    connect(this, &Controller::file_opened,
        save_, &gui_command::Save::set_filename);

    connect(this, &Controller::item_tree_model_created,
        edit_, &gui_command::Edit::set_model);

    connect(this, &Controller::item_tree_model_created,
        add_, &gui_command::Add::set_model);

    connect(this, &Controller::item_tree_model_created,
        clipboard_copy_, &gui_command::Clipboard_Copy::set_model);
    connect(this, &Gui_Controller::selection_model_changed,
        clipboard_copy_, &gui_command::Clipboard_Copy::set_selection_model);
    connect(this, &Controller::item_tree_model_created,
        clipboard_cut_, &gui_command::Clipboard_Cut::set_model);
    connect(this, &Gui_Controller::selection_model_changed,
        clipboard_cut_, &gui_command::Clipboard_Cut::set_selection_model);
    connect(this, &Controller::item_tree_model_created,
        clipboard_paste_, &gui_command::Clipboard_Paste::set_model);
    connect(this, &Gui_Controller::selection_model_changed,
        clipboard_paste_, &gui_command::Clipboard_Paste::set_selection_model);

    connect(this, &Controller::item_tree_model_created,
        display_subtree_, &gui_command::Display_Subtree::set_model);
    connect(this, &Gui_Controller::selection_model_changed,
        display_subtree_, &gui_command::Display_Subtree::set_selection_model);
    connect(display_subtree_,
        &gui_command::Display_Subtree::subtree_window_created,
        this, &Gui_Controller::subtree_window_created);
  }
  void Gui_Controller::open(const QString &filename)
  {
    open_->open(filename);
  }

  void Gui_Controller::select_open()
  {
    select_open_->open();
  }
  void Gui_Controller::save()
  {
    save_->save();
  }
  void Gui_Controller::select_save()
  {
    save_->select_save();
  }
  void Gui_Controller::select_save_copy()
  {
    save_->select_save_copy();
  }
  void Gui_Controller::clipboard_cut()
  {
    clipboard_cut_->cut();
  }
  void Gui_Controller::clipboard_copy()
  {
    clipboard_copy_->copy();
  }
  void Gui_Controller::clipboard_paste()
  {
    clipboard_paste_->paste();
  }
  void Gui_Controller::clipboard_paste_as_child()
  {
    clipboard_paste_->paste_as_child();
  }
  void Gui_Controller::display_subtree()
  {
    display_subtree_->display();
  }
  void Gui_Controller::add_child(const QModelIndex &i)
  {
    add_->add_child(i);
  }
  void Gui_Controller::add_sibling(const QModelIndex &i)
  {
    add_->add_sibling(i);
  }
  void Gui_Controller::edit(const QModelIndex &i)
  {
    edit_->edit(i);
  }

}
