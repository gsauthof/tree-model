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
#include "open.hh"

#include <QCoreApplication>
#include <QThread>

#include <tree_model/item_adaptor.hh>
#include <tree_model/xml.hh>

namespace editor {
  namespace command {

    Open::Open(QObject *parent) : QObject(parent)
    {

    }

    void Open::open(const QString &filename)
    {
      try {
        xxxml::doc::Ptr doc = xxxml::read_file(filename.toUtf8().data());

        // In the non-async case, this should be an no-op
        // otherwise it is necessary such that it can be 
        // adaptaped by the main thread (i.e. the thread
        // that calls setParent())
        tree_model::XML *m = new tree_model::XML(std::move(doc));
        // We don't call moveToThread() on m, because m is
        // getting adopted by a, and moveThread() works
        // recursively.
        //m->moveToThread(QApplication::instance()->thread());
        tree_model::Item_Adaptor *a = new tree_model::Item_Adaptor(m);
        if (QCoreApplication::instance())
          a->moveToThread(QCoreApplication::instance()->thread());
        // for debugging purposes:
        QThread::msleep(delay_);
        if (receivers(SIGNAL(item_tree_model_created(QAbstractItemModel *)))) {
          emit item_tree_model_created(a);
          emit tree_model_created(m);
          emit file_opened(filename);
          emit msg_produced(tr("Opened %1").arg(filename));
        }
        else
          delete a;
      } catch (const std::exception &e) {
        emit failed(QString(e.what()));
      }
      emit finished();
    }
    void Open::set_delay(unsigned msecs)
    {
      delay_ = msecs;
    }

  } // namespace command
} // namespace editor

