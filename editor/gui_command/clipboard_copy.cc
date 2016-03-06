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
#include "clipboard_copy.hh"

#include <QApplication>
#include <QClipboard>
#include <QItemSelectionModel>
#include <QDebug>
#include <QMimeData>

using namespace std;

namespace editor {
  namespace gui_command {

    Clipboard_Copy::Clipboard_Copy(QObject *parent)
      : QObject(parent)
    {
    }

    void Clipboard_Copy::set_model(const QAbstractItemModel *model)
    {
      model_ = model;
    }

    void Clipboard_Copy::copy(const QModelIndexList &is)
    {
      try {
        QClipboard *cb = QApplication::clipboard();

        auto d = model_->mimeData(is);
        // QMimeData can hold multiple variants,
        // for different mime types.
        // They also show up in the clipboard, a pasting application
        // can choose which to pick.
        d->setData("text/plain", d->data("text/xml"));
        auto e = new QMimeData();
        e->setData("text/plain", d->data("text/xml"));

        cb->setMimeData(d);
        // Only relevant on X11:
        // also set the primary selection
        cb->setMimeData(e, QClipboard::Selection);
      } catch (const underflow_error &e) {
        // do nothing
      }
    }



  } // namespace gui_command
} // namespace editor
