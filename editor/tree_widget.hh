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
#ifndef TREE_WIDGET_HH
#define TREE_WIDGET_HH

#include <QAbstractItemModel>
#include <QWidget>

namespace editor {

  namespace Ui {
    class Tree_Widget;
  }
  class Tree_View;

  class Tree_Widget : public QWidget
  {
    Q_OBJECT

    public:
      explicit Tree_Widget(QWidget *parent = nullptr);
      ~Tree_Widget();
      Tree_View &tree_view();

    public slots:
      void jump_to_index(const QModelIndex &i);


    private:
      Ui::Tree_Widget *ui {nullptr};

      void init_slider_bar(const QModelIndex &current);

    private slots:
      void jump_to_rank(int value);

    signals:
      void something_selected(bool b);
  };

}

#endif // TREE_WIDGET_HH
