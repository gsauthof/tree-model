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

#include "gui_controller.hh"

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
  }

  Tree_Widget::~Tree_Widget()
  {
    delete ui;
  }

  void Tree_Widget::set_controller(Gui_Controller *controller)
  {
    if (controller_) {
      disconnect(controller_, &Gui_Controller::item_tree_model_created,
          ui->tree_view, &Tree_View::set_model);
      disconnect(ui->tree_view, &editor::Tree_View::context_requested,
          controller_, &Gui_Controller::display_tree_context);
    }
    controller_ = controller;

    connect(controller_, &Gui_Controller::item_tree_model_created,
        ui->tree_view, &Tree_View::set_model);

    connect(ui->tree_view, &editor::Tree_View::context_requested,
        controller_, &Gui_Controller::display_tree_context);
    connect(ui->tree_view, &editor::Tree_View::remove_triggered,
        controller, &Controller::remove);

    connect(ui->tree_view, &editor::Tree_View::selection_model_changed,
        controller, &Gui_Controller::selection_model_changed);

  }

}
