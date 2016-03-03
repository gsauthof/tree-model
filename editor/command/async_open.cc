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
#include "async_open.hh"

#include <QThread>
#include <QAbstractItemModel>
#include <tree_model/base.hh>

#include "open.hh"
#include <editor/file_type.hh>

namespace editor {
  namespace command {

    Async_Open::Async_Open(QObject *parent)
      : QObject(parent)
    {
      init();
    }
    void Async_Open::init()
    {
      thread_ = new QThread;
      open_   = new Open;
      open_->moveToThread(thread_);

      connect(thread_, &QThread::finished, open_,   &QObject::deleteLater);
      connect(thread_, &QThread::finished, thread_, &QObject::deleteLater);

      connect(this, &Async_Open::open_requested,
          open_, &Open::open);
      connect(this, &Async_Open::open_ft_requested,
          open_, &Open::open_ft);

      connect(open_, &Open::item_tree_model_created,
          this, &Async_Open::item_tree_model_created);
      connect(open_, &Open::tree_model_created,
          this, &Async_Open::tree_model_created);
      connect(open_, &Open::failed,      this, &Async_Open::failed);
      connect(open_, &Open::msg_produced,this, &Async_Open::msg_produced);
      connect(open_, &Open::file_opened, this, &Async_Open::file_opened);
      connect(open_, &Open::finished,    this, &Async_Open::finished);

      thread_->start();
    }

    Async_Open::~Async_Open()
    {
      thread_->quit();
      // we don't want to wait forever e.g. on exit, for a pending open ...
      // (otherwise, if we don't wait at all we get a memory leak because
      // deleteLater is not executed)
      if (thread_->wait(100))
        delete thread_;
    }

    void Async_Open::ignore_result()
    {
      if (!open_)
        return;
      disconnect(open_, &Open::item_tree_model_created,
          this, &Async_Open::item_tree_model_created);
      disconnect(open_, &Open::tree_model_created,
          this, &Async_Open::tree_model_created);
      disconnect(open_, &Open::failed,      this, &Async_Open::failed);
      disconnect(open_, &Open::msg_produced,this, &Async_Open::msg_produced);
      disconnect(open_, &Open::file_opened, this, &Async_Open::file_opened);
      disconnect(open_, &Open::finished,    this, &Async_Open::finished);
      thread_->quit();
      // we don't want to wait forever e.g. for a pending open ...
      if (thread_->wait(100))
        delete thread_;
      init();
      emit finished();
    }

    void Async_Open::open(const QString &filename)
    {
      emit open_requested(filename);
    }
    void Async_Open::open_ft(const QString &filename, const File_Type &ft)
    {
      emit open_ft_requested(filename, ft);
    }
    void Async_Open::set_delay(unsigned msecs)
    {
      open_->set_delay(msecs);
    }



  } // namespace command
} // namespace editor
