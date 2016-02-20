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
#include "edit.hh"

#include <editor/child_dialog.hh>

#include <QAbstractItemModel>

namespace editor {
  namespace gui_command {

    Edit::Edit(QWidget *parent)
      :
        QObject(static_cast<QObject*>(parent)),
        parent_widget_(parent)
    {
    }
    void Edit::edit(const QModelIndex &index)
    {
      if (!model_)
        return;
      if (!index.isValid())
        return;

      QModelIndex key(model_->index(index.row(), 0, index.parent()));
      QModelIndex value(model_->index(index.row(), 1, index.parent()));
      bool has_children = model_->hasChildren(key);
      QString v;
      if (!has_children)
        v = model_->data(value, Qt::EditRole).toString();

      editor::Child_Dialog d(parent_widget_);
      d.setWindowTitle(tr("Edit node"));
      d.set_data(model_->data(key, Qt::EditRole).toString(), v);
      d.enable_value(!has_children);
      if (d.exec()) {
        model_->setData(key, d.key());
        if (!has_children)
          model_->setData(value, d.value());
      }
    }
    void Edit::set_model(QAbstractItemModel *model)
    {
      model_ = model;
    }

  }
}
