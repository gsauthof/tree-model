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
#include "tree_view.hh"

#include <QContextMenuEvent>
#include <QDebug>
#include <QMenu>
#include <QAction>

namespace editor {

  Tree_View::Tree_View(QWidget *parent)
    :
      QTreeView(parent)
  {
    remove_action_ = new QAction(tr("&Remove"), this);
    connect(remove_action_, &QAction::triggered,
        [this](){ emit remove_triggered(selectedIndexes()); });
    edit_action_ = new QAction(tr("&Edit..."), this);
    connect(edit_action_, &QAction::triggered,
        [this](){ emit edit_triggered(context_index_); });
    add_child_action_ = new QAction(tr("&Add child..."), this);
    connect(add_child_action_, &QAction::triggered,
        this, &Tree_View::trigger_add_child);
    add_sibling_action_ = new QAction(tr("Add &sibling..."), this);
    connect(add_sibling_action_, &QAction::triggered,
        this, &Tree_View::trigger_add_sibling);
  }

  QModelIndex Tree_View::index_for_update()
  {
    if (context_menu_visible_)
      return context_index_;
    else {
      auto is = selectedIndexes();
      if (is.empty())
        return QModelIndex();
      else
        return is.front();
    }
  }

  void Tree_View::trigger_add_child()
  {
    emit add_child_triggered(index_for_update()); 
  }
  void Tree_View::trigger_add_sibling()
  {
    emit add_sibling_triggered(index_for_update()); 
  }
  void Tree_View::trigger_remove()
  {
    auto selected_indexes = selectedIndexes();
    emit remove_triggered(selected_indexes);
  }
  void Tree_View::trigger_edit()
  {
    emit edit_triggered(index_for_update()); 
  }

  void Tree_View::contextMenuEvent(QContextMenuEvent *event)
  {
    if (!model())
      return;
    context_index_ = indexAt(event->pos());
    auto global_pos = event->globalPos();

    QMenu menu(parentWidget());
    edit_action_->setEnabled(context_index_.isValid());
    menu.addAction(edit_action_);
    add_sibling_action_->setEnabled(context_index_.isValid());
    menu.addAction(add_sibling_action_);
    add_child_action_->setEnabled(context_index_.isValid()
        || !model()->rowCount(context_index_));
    menu.addAction(add_child_action_);
    menu.addSeparator();
    remove_action_->setEnabled(!selectedIndexes().empty());
    menu.addAction(remove_action_);

    context_menu_visible_ = true;
    menu.exec(global_pos);
    context_menu_visible_ = false;

    context_index_ = QModelIndex();
  }

  void Tree_View::keyPressEvent(QKeyEvent *event)
  {
    if (event->key() == Qt::Key_Delete) {
      trigger_remove();
    } else {
      QTreeView::keyPressEvent(event);
    }
  }

  void Tree_View::set_model(QAbstractItemModel *model)
  {
    setModel(model);
    auto font_metrics = fontMetrics();
    setColumnWidth(0, font_metrics.averageCharWidth()*32);

    emit selection_model_changed(selectionModel());
  }

  void Tree_View::selectionChanged(const QItemSelection &selected,
      const QItemSelection & deselected)
  {
    QTreeView::selectionChanged(selected, deselected);
    emit selection_changed(selected, deselected);
    emit something_selected(!selected.empty());
  }
  void Tree_View::currentChanged(const QModelIndex &current,
      const QModelIndex &previous)
  {
    QTreeView::currentChanged(current, previous);
    emit current_changed(current, previous);
  }

} // namespace editor

