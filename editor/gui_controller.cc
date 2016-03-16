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
#include <editor/gui_command/write_aci.hh>
#include <editor/subtree_window.hh>
#include <tree_model/base.hh>
#include <tree_model/recorder.hh>

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
      display_subtree_(new gui_command::Display_Subtree(parent_widget_)),
      write_aci_      (new gui_command::Write_ACI(parent_widget_))
  {
    connect_open_action();
    connect_select_open_action();
    connect_save_action();
    connect_edit_action();
    connect_add_action();
    connect_clipboard();
    connect_subtree_action();
    connect_write_aci_action();
  }
  void Gui_Controller::connect_open_action()
  {
    connect(open_, &gui_command::Open::item_tree_model_created,
            this, &Gui_Controller::item_tree_model_created);
    connect(open_, &gui_command::Open::tree_model_created,
            this, &Gui_Controller::tree_model_created);
    connect(open_, &gui_command::Open::file_opened,
            this, &Gui_Controller::file_opened);
    connect(open_, &gui_command::Open::file_type_opened,
            this, &Gui_Controller::file_type_opened);
    connect(open_, &gui_command::Open::msg_produced,
            this, &Gui_Controller::msg_produced);
    connect(open_, &gui_command::Open::open_more_urls_requested,
            this, &Gui_Controller::open_more_urls_requested);
    connect(this, &Controller::model_changed,
            open_, &gui_command::Open::set_modified);
  }
  void Gui_Controller::connect_select_open_action()
  {
    connect(select_open_,
        &gui_command::Select_Open::item_tree_model_created,
            this, &Gui_Controller::item_tree_model_created);
    connect(select_open_,
        &gui_command::Select_Open::tree_model_created,
            this, &Gui_Controller::tree_model_created);
    connect(select_open_, &gui_command::Select_Open::file_opened,
            this, &Gui_Controller::file_opened);
    connect(select_open_, &gui_command::Select_Open::file_type_opened,
            this, &Gui_Controller::file_type_opened);
    connect(select_open_, &gui_command::Select_Open::msg_produced,
            this, &Gui_Controller::msg_produced);
  }
  void Gui_Controller::connect_save_action()
  {
    connect(save_, &gui_command::Save::msg_produced,
            this, &Gui_Controller::msg_produced);
    connect(save_, &gui_command::Save::saved,
        this, &Controller::saved);

    connect(this, &Controller::tree_model_created,
            save_, &gui_command::Save::set_tree_model);
    connect(this, &Controller::file_opened,
        save_, &gui_command::Save::set_filename);
    connect(this, &Controller::file_type_opened,
        save_, &gui_command::Save::set_file_type);
  }

  void Gui_Controller::connect_edit_action()
  {
    connect(this, &Controller::item_tree_model_created,
        edit_, &gui_command::Edit::set_model);
    connect(edit_, SIGNAL(begin_transaction_requested(const QString&)),
        recorder_, SLOT(begin_transaction(const QString&)));
    connect(edit_, &gui_command::Edit::commit_requested,
        recorder_, &tree_model::Recorder::commit);
  }

  void Gui_Controller::connect_add_action()
  {
    connect(this, &Controller::item_tree_model_created,
        add_, &gui_command::Add::set_model);
    //connect(add_, &gui_command::Add::begin_transaction_requested,
    //    recorder_, &tree_model::Recorder::begin_transaction);
    connect(add_, SIGNAL(begin_transaction_requested(const QString&)),
        recorder_, SLOT(begin_transaction(const QString&)));
    connect(add_, &gui_command::Add::commit_requested,
        recorder_, &tree_model::Recorder::commit);
  }

  void Gui_Controller::connect_clipboard()
  {
    connect(this, &Controller::item_tree_model_created,
        clipboard_copy_, &gui_command::Clipboard_Copy::set_model);
    connect(this, &Controller::item_tree_model_created,
        clipboard_cut_, &gui_command::Clipboard_Cut::set_model);
    connect(this, &Controller::item_tree_model_created,
        clipboard_paste_, &gui_command::Clipboard_Paste::set_model);
  }

  void Gui_Controller::connect_subtree_action()
  {
    connect(this, &Controller::item_tree_model_created,
        display_subtree_, &gui_command::Display_Subtree::set_model);
    connect(this, &Gui_Controller::selection_model_changed,
        display_subtree_, &gui_command::Display_Subtree::set_selection_model);
    connect(display_subtree_,
        &gui_command::Display_Subtree::subtree_window_created,
        this, &Gui_Controller::subtree_window_created);
    connect(display_subtree_, &gui_command::Display_Subtree::undo_requested,
        this, &Controller::undo);
    connect(display_subtree_, &gui_command::Display_Subtree::redo_requested,
        this, &Controller::redo);
  }

  void Gui_Controller::connect_write_aci_action()
  {
    connect(this, &Controller::item_tree_model_created,
        write_aci_, &gui_command::Write_ACI::set_model);
    connect(this, &Controller::file_type_opened,
        write_aci_, &gui_command::Write_ACI::set_file_type);
    connect(write_aci_, SIGNAL(begin_transaction_requested(const QString&)),
        recorder_, SLOT(begin_transaction(const QString&)));
    connect(write_aci_, &gui_command::Write_ACI::commit_requested,
        recorder_, &tree_model::Recorder::commit);
    connect(write_aci_, &gui_command::Write_ACI::msg_produced,
            this, &Gui_Controller::msg_produced);
  }

  void Gui_Controller::open(const QString &filename)
  {
    open_->open(filename);
  }
  void Gui_Controller::open_urls(const QList<QUrl> &urls)
  {
    open_->open_urls(urls);
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
  void Gui_Controller::clipboard_cut(const QModelIndexList &is)
  {
    clipboard_cut_->cut(is);
  }
  void Gui_Controller::clipboard_copy(const QModelIndexList &is)
  {
    clipboard_copy_->copy(is);
  }
  void Gui_Controller::clipboard_paste(const QModelIndexList &is)
  {
    clipboard_paste_->paste(is);
  }
  void Gui_Controller::clipboard_paste_as_child(const QModelIndexList &is)
  {
    clipboard_paste_->paste_as_child(is);
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
  void Gui_Controller::write_aci()
  {
    write_aci_->write();
  }

}
