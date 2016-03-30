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
#include "arguments.hh"

#include "instance_manager.hh"

#include <QApplication>
#include <QTimer>
#include <QDebug>

#include <memory>

#include "setup.hh"


using namespace std;

int main(int argc, char **argv)
{
  QApplication a(argc, argv);
  a.setApplicationName(QApplication::tr("xmleditor"));
  a.setApplicationVersion("2015-09-26");
  a.setOrganizationDomain(QApplication::tr("georg.so"));
  //a.setOrganizationName("");
  // On most platforms (unix/windows), the application display name
  // is automatically trailing the window title
  // (seperated with a dash)
  a.setApplicationDisplayName(QApplication::tr("XML Editor"));

  editor::Setup setup;

  editor::Arguments args(argc, argv);

  editor::Instance_Manager im;

  args.connect(&args, &editor::Arguments::open_triggered,
                &im, &editor::Instance_Manager::first_open_requested);
  QTimer::singleShot(0, &args, SLOT(parse()));

  return a.exec();
}
