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
#ifndef EDITOR_TREE_VIEW_HH
#define EDITOR_TREE_VIEW_HH

#include <QTreeView>

namespace editor {

class Tree_View : public QTreeView
{
  Q_OBJECT
  public:
    Tree_View(QWidget *parent = nullptr);

    void contextMenuEvent(QContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

  signals:
    void context_requested(const QPoint &global_pos,
        const QModelIndex &context_index,
        const QModelIndexList &selected_indexes);
    void remove_triggered(const QModelIndexList &selected_indexes);

  public slots:
    void set_model(QAbstractItemModel *model);

};

} // namespace editor

#endif // EDITOR_TREE_VIEW_HH
