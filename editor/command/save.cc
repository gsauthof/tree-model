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
#include "save.hh"

#include <QDebug>
#include <QThread>

#include <tree_model/base.hh>

#include <exception>
using namespace std;

namespace editor {
  namespace command {


    Save::Save(QObject *parent)
      :
        QObject(parent)
    {
    }
    void Save::save()
    {
      if (filename_.isEmpty()) {
        QString m("no filename provided (save)");
        emit failed(m);
        return;
      }
      save_as(filename_);
    }
    void Save::save_as(const QString &filename)
    {
      if (model_) {
        try {
          model_->save(filename);
          QThread::msleep(delay_);
          emit saved(filename);
          emit msg_produced(tr("Successfully saved %1").arg(filename));
        } catch (const exception &e) {
          QString m(e.what());
          emit failed(m);
        }
      } else {
        QString m("No file opened");
        emit failed(m);
      }
      emit finished();
    }
    void Save::set_tree_model(tree_model::Base *model)
    {
      model_ = model;
    }
    void Save::set_filename(const QString &filename)
    {
      filename_ = filename;
    }
    const QString &Save::filename() const
    {
      return filename_;
    }
    void Save::set_delay(unsigned msecs)
    {
      delay_ = msecs;
    }

  }
}
