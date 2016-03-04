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
#include <QAbstractItemModel>

#include <exception>
#include <stdexcept>

#include <editor/file_type.hh>

#include "open_xml.hh"
#include "open_ber.hh"

using namespace std;

namespace editor {
  namespace command {

    Open::Open(QObject *parent)
      : QObject(parent)
    {
    }

    void Open::open(const QString &filename)
    {
      // TODO implement content based autodetection?
      open_ft(filename, filename.toLower().endsWith(".xml") ?
          File_Type(File_Type::XML) : File_Type(File_Type::BER));
    }
    void Open::open_ft(const QString &filename, const File_Type &ft_P)
    {
      try {
        File_Type ft(ft_P);
        tree_model::Base *m = nullptr;
        QAbstractItemModel *a = nullptr;
        deque<string> asn_filenames;
        switch (ft.major()) {
          case File_Type::XML:
            {
              auto p = open_xml(filename);
              a = p.first;
              m = p.second;
            }
            break;
          case File_Type::BER:
            std::tie(a, m, asn_filenames) = open_ber(filename);
            ft.set_asn_filenames(std::move(asn_filenames));
            break;
          default:
            throw std::logic_error("File Type not implemented yet");
        }
        // In the non-async case, this should be an no-op
        // otherwise it is necessary such that it can be 
        // adaptaped by the main thread (i.e. the thread
        // that calls setParent())
        if (QCoreApplication::instance())
          a->moveToThread(QCoreApplication::instance()->thread());
        // for debugging purposes:
        QThread::msleep(delay_);
        if (receivers(SIGNAL(item_tree_model_created(QAbstractItemModel *)))) {
          emit item_tree_model_created(a);
          emit tree_model_created(m);
          emit file_opened(filename);
          emit file_type_opened(ft);
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

