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
#include "display_tree_context.hh"


#include <QMenu>
#include <QAction>
#include <QDebug>

namespace editor {
  namespace gui_command {

    Display_Tree_Context::Display_Tree_Context(QWidget *parent)
      :
        QObject(static_cast<QObject*>(parent)),
        parent_widget_(parent)
    {
      remove_action_ = new QAction(tr("&Remove"), this);
      connect(remove_action_, &QAction::triggered,
          this, &Display_Tree_Context::remove);
      edit_action_ = new QAction(tr("&Edit..."), this);
      connect(edit_action_, &QAction::triggered,
          this, &Display_Tree_Context::edit);
      add_action_ = new QAction(tr("&Add child..."), this);
      connect(add_action_, &QAction::triggered,
          this, &Display_Tree_Context::add);
    }
    void Display_Tree_Context::display(const QPoint &global_pos,
        const QModelIndex &context_index,
        const QModelIndexList &selected_indexes)
    {
      //qDebug() << "context index: " << context_index;
      //qDebug() << "selected indexes: " << selected_indexes;

      context_index_ = &context_index;
      selected_indexes_ = &selected_indexes;

      QMenu menu(parent_widget_);
      menu.addAction(edit_action_);
      menu.addAction(add_action_);
      menu.addAction(remove_action_);

      menu.exec(global_pos);

      context_index_    = nullptr;
      selected_indexes_ = nullptr;
    }
    void Display_Tree_Context::remove()
    {
      if (!model_ || !selected_indexes_ || selected_indexes_->isEmpty()) {
        return;
      }
      emit remove_triggered(*selected_indexes_);
    }
    void Display_Tree_Context::edit()
    {
      if (!model_ || !context_index_)
        return;
      emit edit_triggered(*context_index_);
    }
    void Display_Tree_Context::add()
    {
      if (!model_ || !context_index_)
        return;
      emit add_triggered(*context_index_);
    }
    void Display_Tree_Context::set_item_tree_model(QAbstractItemModel *model)
    {
      model_ = model;
    }

  }
}
