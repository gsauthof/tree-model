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
#include "controller.hh"

#include <QDebug>

#include <editor/command/new.hh>
#include <editor/command/async_open.hh>
#include <editor/command/async_save.hh>
#include <editor/command/remove.hh>
#include <tree_model/base.hh>
#include <tree_model/recorder.hh>

namespace editor {

  Controller::Controller(QObject *parent)
    :
      QObject(parent),
      new_     (new command::New(this)),
      open_    (new command::Async_Open(this)),
      save_    (new command::Async_Save(this)),
      remove_  (new command::Remove(this)),
      recorder_(new tree_model::Recorder(this))
  {
    connect(new_, &command::New::item_tree_model_created,
            this, &Controller::item_tree_model_created);
    connect(new_, &command::New::tree_model_created,
            this, &Controller::tree_model_created);

    connect(open_, &command::Async_Open::item_tree_model_created,
            this, &Controller::item_tree_model_created);
    connect(open_, &command::Async_Open::tree_model_created,
            this, &Controller::tree_model_created);

    connect(open_, &command::Async_Open::file_opened,
            this, &Controller::file_opened);

    connect(open_, &command::Async_Open::msg_produced,
        this, &Controller::msg_produced);

    connect(save_, &command::Async_Save::msg_produced,
        this, &Controller::msg_produced);
    connect(save_, &command::Async_Save::saved,
        this, &Controller::saved);

    connect(this, &Controller::item_tree_model_created,
            this, &Controller::set_item_tree_model);
    connect(this, &Controller::tree_model_created,
            this, &Controller::set_tree_model);

    connect(this, &Controller::tree_model_created,
            save_, &command::Async_Save::set_tree_model);
    connect(this, &Controller::file_opened,
        save_, &command::Async_Save::set_filename);

    connect(this, &Controller::item_tree_model_created,
        remove_, &command::Remove::set_model);

    connect(this, &Controller::item_tree_model_created,
        recorder_, &tree_model::Recorder::set_model);
    // we use old signal/slot syntax because we ignore signal argument
    connect(this, SIGNAL(saved(const QString&)),
        recorder_, SLOT(register_saved()));
    connect(recorder_, &tree_model::Recorder::rewind_enabled,
        this, &Controller::rewind_enabled);
    connect(recorder_, &tree_model::Recorder::forward_enabled,
        this, &Controller::forward_enabled);
    connect(recorder_, &tree_model::Recorder::model_changed,
        this, &Controller::model_changed);
  }
  QAbstractItemModel *Controller::item_tree_model()
  {
    return item_tree_model_;
  }
  void Controller::open(const QString &filename)
  {
    open_->open(filename);
  }
  void Controller::save()
  {
    save_->save();
  }
  void Controller::remove(const QModelIndexList &l)
  {
    remove_->remove(l);
  }
  void Controller::undo()
  {
    recorder_->rewind();
  }
  void Controller::redo()
  {
    recorder_->forward();
  }

  void Controller::set_item_tree_model(QAbstractItemModel *model)
  {
    if (item_tree_model_)
      item_tree_model_->deleteLater();
    item_tree_model_ = model;
    if (item_tree_model_)
      item_tree_model_->setParent(this);
  }
  void Controller::set_tree_model(tree_model::Base *model)
  {
    if (tree_model_)
      tree_model_->deleteLater();
    tree_model_ = model;
    // don't reparent, since it is owned by the adaptor ...
  }

  void Controller::request_empty_model()
  {
    new_->create();
  }

}
