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
#include <QDataWidgetMapper>

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

      editor::Child_Dialog d(parent_widget_);
      d.setWindowTitle(tr("Edit node"));

      bool has_children = model_->hasChildren(
          model_->index(index.row(), 0, index.parent()));
      // the mapper doesn't do the disabling for us, i.e.
      // it doesn't query
      // QAbstractItemModel::flags & Qt::ItemIsEditable
      // apparently
      d.value_line().setEnabled(!has_children);

      QDataWidgetMapper mapper;
      mapper.setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
      mapper.setModel(model_);
      mapper.setRootIndex(index.parent());
      mapper.addMapping(&d.key_line(), 0);
      mapper.addMapping(&d.value_line(), 1);
      mapper.setCurrentIndex(index.row());

      if (d.exec()) {
        emit begin_transaction_requested(tr("edit node"));
        mapper.submit();
        emit commit_requested();
      }
    }
    void Edit::set_model(QAbstractItemModel *model)
    {
      model_ = model;
    }

  }
}
