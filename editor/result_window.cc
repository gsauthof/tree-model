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

#include "result_window.hh"
#include "ui_result_window.h"

namespace editor {

  Result_Window::Result_Window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Result_Window)
  {
    ui->setupUi(this);
  }

  Table_View &Result_Window::table_view()
  {
    return *ui->widget;
  }

  Result_Window::~Result_Window()
  {
    delete ui;
  }

} // namespace editor
