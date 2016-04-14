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

#include "subtree_window.hh"
#include "ui_subtree_window.h"

#include <QDebug>

namespace editor {

  Subtree_Window::Subtree_Window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Subtree_Window)
  {
    ui->setupUi(this);
  }

  Tree_Widget &Subtree_Window::tree_widget()
  {
    return *ui->widget;
  }

  Subtree_Window::~Subtree_Window()
  {
    delete ui;
  }

} // namespace editor
