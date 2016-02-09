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


#include "instance_manager.hh"
#include "instance.hh"

using namespace std;

namespace editor {
  
  Instance_Manager::Instance_Manager(QObject *parent)
    :
      QObject(parent)
  {
    auto i = create_instance();
    connect(this, &Instance_Manager::first_open_requested,
        i, &Instance::open);
  }
  Instance *Instance_Manager::create_instance()
  {
    auto i = new Instance(this);
    instances_.push_back(i);
    connect(i, &Instance::quit_requested, this, &Instance_Manager::quit);
    connect(i, &Instance::new_requested,
        this, &Instance_Manager::create_instance);
    i->show();
    return i;
  }
  void Instance_Manager::do_create_instance()
  {
    create_instance();
  }
  void Instance_Manager::quit()
  {
    // variation: build a list of modified windows and
    // display them in one dialog such that they can be saved
    // discarded en masse
    // cf. isWindowModified()
    for (auto i : instances_)
      i->close();
  }
  // cf. QApplication::lastWindowClosed() signal

}
