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


#include "instance.hh"

#include "main_window.hh"
#include "gui_controller.hh"
#include "vc_matchmaker.hh"

#include <QAction>

namespace editor {

  Instance::Instance(QObject *parent)
    :
      QObject(parent),
      main_window_(new Main_Window),
      gui_controller_(new Gui_Controller(main_window_))
  {
    connect_view_controller(*main_window_, *gui_controller_);
    connect(main_window_, &Main_Window::quit_triggered,
        this, &Instance::quit_requested);
    connect(main_window_, &Main_Window::new_triggered,
        this, &Instance::new_requested);
    gui_controller_->request_empty_model();
  }
  void Instance::open(const QString &filename)
  {
    gui_controller_->open(filename);
  }
  void Instance::show()
  {
    main_window_->show();
  }
  void Instance::close()
  {
    main_window_->close();
  }
  Instance::~Instance()
  {
    delete main_window_;
  }
  Main_Window *Instance::main_window()
  {
    return main_window_;
  }

}
