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

#include "clipboard_paste.hh"

#include <QApplication>
#include <QClipboard>
#include <QItemSelectionModel>
#include <QMimeData>

namespace editor {
  namespace gui_command {

    void Clipboard_Paste::set_model(QAbstractItemModel *model)
    {
      model_ = model;
    }
    void Clipboard_Paste::set_selection_model(const QItemSelectionModel *smodel)
    {
      smodel_ = smodel;
    }

    void Clipboard_Paste::paste()
    {
      paste(true);
    }
    void Clipboard_Paste::paste_as_child()
    {
      paste(false);
    }

    void Clipboard_Paste::paste(bool insert_before)
    {
      if (!model_ || !smodel_)
        return;
      auto is = smodel_->selectedIndexes();
      QModelIndex i;
      if (is.empty()) {
        if (model_->index(0, 0).isValid())
          return;
        insert_before = false;
      } else {
        i = is.front();
      }

      QClipboard *cb = QApplication::clipboard();
      auto d = cb->mimeData();
      if (!d->hasFormat("text/xml"))
        return;
      if (insert_before)
        model_->dropMimeData(d, Qt::CopyAction, i.row(), i.column(),
            i.parent());
      else
        model_->dropMimeData(d, Qt::CopyAction, -1, -1, i);
    }


  } // namespace gui_command
} // namespace editor
