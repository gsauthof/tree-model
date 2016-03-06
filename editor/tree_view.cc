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

#include <deque>

#include <tree_model/util.hh>

using namespace std;

namespace editor {

  Tree_View::Tree_View(QWidget *parent)
    :
      QTreeView(parent)
  {
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    // or:
    // setDragDropMode(QAbstractItemView::DragDrop);
    // ?
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);

    // these are just temporary actions
    // they are replaced by the Main_Window ones when the
    // widget is setup inside the main window
    remove_action_ = new QAction(tr("&Remove"), this);
    set_remove_action(remove_action_);
    edit_action_ = new QAction(tr("&Edit..."), this);
    set_edit_action(edit_action_);
    add_child_action_ = new QAction(tr("&Add child..."), this);
    set_add_child_action(add_child_action_);
    add_sibling_action_ = new QAction(tr("Add &sibling..."), this);
    set_add_sibling_action(add_sibling_action_);
  }

  void Tree_View::set_remove_action(QAction *a)
  {
    remove_action_ = a;
    connect(remove_action_, &QAction::triggered,
        this, &Tree_View::trigger_remove);
  }
  void Tree_View::set_edit_action(QAction *a)
  {
    edit_action_ = a;
    connect(edit_action_, &QAction::triggered,
        this, &Tree_View::trigger_edit);
  }
  void Tree_View::set_add_child_action(QAction *a)
  {
    add_child_action_ = a;
    connect(add_child_action_, &QAction::triggered,
        this, &Tree_View::trigger_add_child);
  }
  void Tree_View::set_add_sibling_action(QAction *a)
  {
    add_sibling_action_ = a;
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
    // those actions are usually also part of the main window's
    // menu, thus changing the state here effects the menu as well
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

    breadth_first_expand(20);

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

  void Tree_View::breadth_first_expand(unsigned n)
  {
    auto m = model();
    if (!m)
      return;
    auto root = m->index(0, 0);
    tree_model::util::breadth_first(root, n, [this](const QModelIndex &p) {
        expand(p); });
  }

  void Tree_View::breadth_first_collapse(unsigned n)
  {
    auto m = model();
    if (!m)
      return;
    auto root = m->index(0, 0);
    tree_model::util::breadth_first(root, n, [this](const QModelIndex &p) {
        collapse(p); });
  }

} // namespace editor

