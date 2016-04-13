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

#include "table_view.hh"

#include <QHeaderView>

namespace editor {

  void Table_View::currentChanged(const QModelIndex &current,
      const QModelIndex &previous)
  {
    QTableView::currentChanged(current, previous);
    emit current_changed(current, previous);
  }
  void Table_View::setModel(QAbstractItemModel *model)
  {
    QTableView::setModel(model);
    resizeColumnsToContents();
    horizontalHeader()->setStretchLastSection(true);
  }

} // editor
