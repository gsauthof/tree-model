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
#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

#include <QApplication>

static char arg0[10] = "fake";
static char *fake_argv[2] = {arg0};
static int fake_argc = 1;

int main(int argc, char **argv)
{
  // without QApplication, widgets can segfault
  // on uninitialized singleton data ...
  // Also, QApplication must be constructed once, constructing it e.g.
  // in each test case yields segfaults ...
  QApplication app(fake_argc, fake_argv);
  app.setAttribute(Qt::AA_Use96Dpi, true);
  int result = Catch::Session().run( argc, argv );
  return result;
}
