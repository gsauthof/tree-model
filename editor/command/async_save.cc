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
#include "async_save.hh"

#include <QDebug>
#include <QThread>
#include "save.hh"

namespace editor {

  namespace command {

    Async_Save::Async_Save(QObject *parent)
      :
        QObject(parent)
    {
      thread_ = new QThread;
      save_   = new Save;
      save_->moveToThread(thread_);

      connect(thread_, &QThread::finished,
          save_, &QObject::deleteLater);
      connect(thread_, &QThread::finished,
          thread_, &QObject::deleteLater);

      connect(this, &Async_Save::save_requested,
          save_, &Save::save);
      // We don't overload save because we want to use
      // Qt 5 new signal slot syntax ... otherwise we had to use
      // SLOT(save(const QString&)) ...
      connect(this, &Async_Save::save_as_requested, save_, &Save::save_as);
      connect(this, &Async_Save::save_copy_as_requested,
          save_, &Save::save_copy_as);

      connect(save_, &Save::msg_produced, this, &Async_Save::msg_produced);
      connect(save_, &Save::failed,       this, &Async_Save::failed);
      connect(save_, &Save::saved,        this, &Async_Save::saved);
      connect(save_, &Save::finished,     this, &Async_Save::finished);

      thread_->start();
    }
    Async_Save::~Async_Save()
    {
      thread_->quit();
      thread_->wait();
      delete thread_;
    }

    void Async_Save::save()
    {
      emit save_requested();
    }
    void Async_Save::save_as(const QString &filename)
    {
      emit save_as_requested(filename);
    }
    void Async_Save::save_copy_as(const QString &filename)
    {
      emit save_copy_as_requested(filename);
    }

    void Async_Save::set_tree_model(tree_model::Base *model)
    {
      save_->set_tree_model(model);
    }
    void Async_Save::set_filename(const QString &filename)
    {
      save_->set_filename(filename);
    }
    void Async_Save::set_file_type(const File_Type &file_type)
    {
      save_->set_file_type(file_type);
    }
    const QString &Async_Save::filename() const
    {
      return save_->filename();
    }
    void Async_Save::set_delay(unsigned msecs)
    {
      save_->set_delay(msecs);
    }


  }

}
