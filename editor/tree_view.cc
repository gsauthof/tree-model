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

#include "qt_awesome.hh"

#include <tree_model/util.hh>

#include <QContextMenuEvent>
#include <QDebug>
#include <QMenu>
#include <QAction>

#include <deque>


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

    // when added to a main window these action are replaced
    // with the menu ones
    // when added to a subtree view they are used as is
    set_remove_action(new QAction(tr("&Remove"), this));
    set_edit_action(new QAction(tr("&Edit..."), this));
    set_add_child_action(new QAction(tr("&Add child..."), this));
    set_add_sibling_action(new QAction(tr("Add &sibling..."), this));

    set_cut_action(new QAction(tr("Cu&t"), this));
    cut_action_->setShortcut(QKeySequence::Cut);
    cut_action_->setIcon(fa_instance()->icon(fa::scissors));
    // othwise the shortcut is not enabled
    addAction(cut_action_);

    set_copy_action(new QAction(tr("&Copy"), this));
    copy_action_->setShortcut(QKeySequence::Copy);
    copy_action_->setIcon(fa_instance()->icon(fa::clone));
    addAction(copy_action_);

    set_paste_action(new QAction(tr("Paste"), this));
    paste_action_->setShortcut(QKeySequence::Paste);
    paste_action_->setIcon(fa_instance()->icon(fa::clipboard));
    addAction(paste_action_);

    set_paste_as_child_action(new QAction(tr("Paste as c&amp;hild"), this));
    paste_as_child_action_->setShortcut(tr("Ctrl+Shift+V"));
    addAction(paste_as_child_action_);
  }

  void Tree_View::set_remove_action(QAction *a)
  {
    removeAction(remove_action_);
    remove_action_ = a;
    connect(remove_action_, &QAction::triggered,
        this, &Tree_View::trigger_remove);
  }
  void Tree_View::set_edit_action(QAction *a)
  {
    removeAction(edit_action_);
    edit_action_ = a;
    connect(edit_action_, &QAction::triggered,
        this, &Tree_View::trigger_edit);
  }
  void Tree_View::set_add_child_action(QAction *a)
  {
    removeAction(add_child_action_);
    add_child_action_ = a;
    connect(add_child_action_, &QAction::triggered,
        this, &Tree_View::trigger_add_child);
  }
  void Tree_View::set_add_sibling_action(QAction *a)
  {
    removeAction(add_sibling_action_);
    add_sibling_action_ = a;
    connect(add_sibling_action_, &QAction::triggered,
        this, &Tree_View::trigger_add_sibling);
  }
  void Tree_View::set_copy_action(QAction *a)
  {
    removeAction(copy_action_);
    copy_action_ = a;
    connect(copy_action_, &QAction::triggered,
        this, &Tree_View::trigger_copy);
  }
  void Tree_View::set_cut_action(QAction *a)
  {
    // othwerwise the shortcut of the old action is still active
    removeAction(cut_action_);
    cut_action_ = a;
    connect(cut_action_, &QAction::triggered,
        this, &Tree_View::trigger_cut);
  }
  void Tree_View::set_paste_action(QAction *a)
  {
    removeAction(paste_action_);
    paste_action_ = a;
    connect(paste_action_, &QAction::triggered,
        this, &Tree_View::trigger_paste);
  }
  void Tree_View::set_paste_as_child_action(QAction *a)
  {
    removeAction(paste_as_child_action_);
    paste_as_child_action_ = a;
    connect(paste_as_child_action_, &QAction::triggered,
        this, &Tree_View::trigger_paste_as_child);
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
  void Tree_View::trigger_cut()
  {
    emit cut_triggered(selectedIndexes());
  }
  void Tree_View::trigger_copy()
  {
    emit copy_triggered(selectedIndexes());
  }
  void Tree_View::trigger_paste()
  {
    emit paste_triggered(selectedIndexes());
  }
  void Tree_View::trigger_paste_as_child()
  {
    emit paste_as_child_triggered(selectedIndexes());
  }

  void Tree_View::contextMenuEvent(QContextMenuEvent *event)
  {
    if (!model())
      return;
    context_index_ = indexAt(event->pos());
    auto global_pos = event->globalPos();

    QMenu menu(parentWidget());
    // if part of a main window those actions are shared with the menu
    edit_action_->setEnabled(context_index_.isValid());
    menu.addAction(edit_action_);
    add_sibling_action_->setEnabled(context_index_.isValid());
    menu.addAction(add_sibling_action_);
    add_child_action_->setEnabled(context_index_.isValid()
        || !model()->rowCount(context_index_));
    menu.addAction(add_child_action_);
    menu.addSeparator();
    // XXX alternatively setEnabled could be connected to the selection
    // signal, on setAction - then it to be disconnected in the main window
    cut_action_->setEnabled(!selectedIndexes().empty());
    menu.addAction(cut_action_);
    copy_action_->setEnabled(!selectedIndexes().empty());
    menu.addAction(copy_action_);
    paste_action_->setEnabled(!selectedIndexes().empty());
    menu.addAction(paste_action_);
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

