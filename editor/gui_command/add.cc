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
#include "add.hh"

#include <editor/child_dialog.hh>

#include <QAbstractItemModel>

namespace editor {
  namespace gui_command {

    Add::Add(QWidget *parent)
      :
        QObject(static_cast<QObject*>(parent)),
        parent_widget_(parent)
    {
    }

    void Add::add_internal(const QModelIndex &index, bool as_child)
    {
      if (!model_)
        return;
      if (!as_child && !index.isValid())
        return;
      // Don't add child to node that has a value
      if (as_child && index.isValid() && !model_->rowCount(index)
          && model_->data(index.sibling(index.row(), 1)).isValid())
        return;
      // Don't add second doc root
      if (as_child && !index.isValid() && model_->rowCount(index))
        return;

      editor::Child_Dialog d(parent_widget_);
      if (as_child)
        d.setWindowTitle(tr("Add child"));
      else
        d.setWindowTitle(tr("Add sibling"));
      if (d.exec()) {
        int row = 0;
        QModelIndex parent;
        if (as_child) {
          row = model_->rowCount(index);
          parent = index;
        } else {
          row = index.row();
          parent = index.parent();
        }
        emit begin_transaction_requested(tr("insert rows"));
        if (model_->insertRow(row, parent)) {
          auto key = model_->index(row, 0, parent);
          auto value = model_->index(row, 1, parent);
          model_->setData(key, d.key());
          model_->setData(value, d.value());
        }
        emit commit_requested();
      }
    }
    void Add::add_child(const QModelIndex &index)
    {
      add_internal(index, true);
    }
    void Add::add_sibling(const QModelIndex &index)
    {
      add_internal(index, false);
    }

    void Add::set_model(QAbstractItemModel *model)
    {
      model_ = model;
    }

  }
}
