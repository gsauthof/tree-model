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

namespace editor {

  Tree_View::Tree_View(QWidget *parent)
    :
      QTreeView(parent)
  {
  }

  void Tree_View::contextMenuEvent(QContextMenuEvent *event)
  {
    auto context_index = indexAt(event->pos());
    auto selected_indexes = selectedIndexes();
    emit context_requested(event->globalPos(),
        context_index,
        selected_indexes);
  }

  void Tree_View::keyPressEvent(QKeyEvent *event)
  {
    if (event->key() == Qt::Key_Delete) {
      auto selected_indexes = selectedIndexes();
      emit remove_triggered(selected_indexes);
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
  }

} // namespace editor

