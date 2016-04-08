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
#include "display_subtree.hh"

#include <QItemSelectionModel>
#include <QShortcut>
#include <QDebug>

#include <editor/subtree_window.hh>
#include <editor/tree_widget.hh>
#include <editor/tree_view.hh>
#include <tree_model/util.hh>

using namespace std;

namespace editor {
  namespace gui_command {

    Display_Subtree::Display_Subtree(QWidget *parent)
      :
        QObject(parent),
        parent_widget_(parent)
    {
    }

    void Display_Subtree::set_model(QAbstractItemModel *model)
    {
      model_ = model;
    }
    void Display_Subtree::set_selection_model(const QItemSelectionModel *smodel)
    {
      smodel_ = smodel;
    }


    void Display_Subtree::display()
    {
      if (!model_ || !smodel_)
        return;
      auto is = smodel_->selectedIndexes();
      if (is.empty())
        return;
      auto i = is.front();
      if (!i.isValid())
        return;

      auto w = new Subtree_Window(parent_widget_);

      auto i0 = i.sibling(i.row(), 0);
      auto title = QString("%1 %2").arg(model_->data(i0)
          .toString()).arg(model_->data(i, Qt::ToolTipRole).toString());
      w->setWindowTitle(title);

      connect(new QShortcut(QKeySequence::Undo, w), &QShortcut::activated,
          this, &Display_Subtree::undo_requested);
      connect(new QShortcut(QKeySequence::Redo, w), &QShortcut::activated,
          this, &Display_Subtree::redo_requested);

      auto &tv = w->tree_widget().tree_view();
      tv.set_model(model_);
      tv.setRootIndex(i0);
      tv.breadth_first_expand(50);

      emit subtree_window_created(w);

      w->show();
    }



  } // namespace gui_command
} // namespace editor
