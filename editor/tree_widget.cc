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
#include "tree_widget.hh"
#include "ui_tree_widget.h"


namespace editor {

  Tree_Widget::Tree_Widget(
      QWidget *parent)
    :
    QWidget(parent),
    ui(new Ui::Tree_Widget)
  {
    ui->setupUi(this);
    connect(ui->tree_view, &Tree_View::something_selected,
        this, &Tree_Widget::something_selected);

    connect(ui->tree_view, &Tree_View::current_changed,
        [this](const QModelIndex &current,
          const QModelIndex &previous) { init_slider_bar(current); });
    connect(ui->slider_bar, &Slider_Bar::jump_requested,
        this, &Tree_Widget::jump_to_rank);
  }

  void Tree_Widget::init_slider_bar(const QModelIndex &current)
  {
    auto m = ui->tree_view->model();
    if (!m) return;
    auto n = m->rowCount(current.parent());
    auto pos = current.row()+1;
    ui->slider_bar->init(pos, n);
  }

  void Tree_Widget::jump_to_rank(int value)
  {
    if (value < 1)
      return;
    auto i = ui->tree_view->currentIndex().sibling(value-1, 0);
    jump_to_index(i);
  }

  void Tree_Widget::jump_to_index(const QModelIndex &i)
  {
    if (!i.isValid())
      return;
    // also selects:
    ui->tree_view->setCurrentIndex(i);
    // does not set the current index:
    //ui->tree_view->selectionModel()->select(i,
    //    QItemSelectionModel::ClearAndSelect);
    ui->tree_view->scrollTo(i, QAbstractItemView::PositionAtCenter);
  }

  Tree_Widget::~Tree_Widget()
  {
    delete ui;
  }

  Tree_View &Tree_Widget::tree_view()
  {
    return *ui->tree_view;
  }

}
