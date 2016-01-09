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
    void Add::add(const QModelIndex &index)
    {
      if (!model_)
        return;

      editor::Child_Dialog d(parent_widget_);
      d.setWindowTitle(tr("Add child"));
      if (d.exec()) {
        int row = model_->rowCount(index);
        if (model_->insertRow(row, index)) {
          auto key = model_->index(row, 0, index);
          auto value = model_->index(row, 1, index);
          model_->setData(key, d.key());
          model_->setData(value, d.value());
        }
      }
    }
    void Add::set_model(QAbstractItemModel *model)
    {
      model_ = model;
    }

  }
}
