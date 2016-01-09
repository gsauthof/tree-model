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
#ifndef DISPLAY_TREE_CONTEXT_HH
#define DISPLAY_TREE_CONTEXT_HH

#include <QObject>
#include <QModelIndex>

class QWidget;
class QAction;
class AbstractItemModel;

namespace editor {
  namespace gui_command {

    class Display_Tree_Context : public QObject {
      Q_OBJECT
      public:
        Display_Tree_Context(QWidget *parent = nullptr);


      public slots:
        void display(const QPoint &global_pos,
            const QModelIndex &context_index,
            const QModelIndexList &selected_indexes);
        void set_item_tree_model(QAbstractItemModel *model);

      signals:
        void remove_triggered(const QModelIndexList &);
        void edit_triggered  (const QModelIndex &);
        void add_triggered   (const QModelIndex &);

      private:
        QWidget               *parent_widget_    {nullptr};
        QAction               *remove_action_    {nullptr};
        QAction               *edit_action_      {nullptr};
        QAction               *add_action_       {nullptr};
        QAbstractItemModel    *model_            {nullptr};
        const QModelIndex     *context_index_    {nullptr};
        const QModelIndexList *selected_indexes_ {nullptr};
      private slots:
        void remove();
        void edit();
        void add();
    };

  }
}


#endif
