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

//#define CATCH_CONFIG_MAIN

#define CATCH_CONFIG_RUNNER

#include <catch.hpp>

#include <QCoreApplication>

int main(int argc, char **argv)
{
  // we need an application object for running event loops, e.g.
  // QEventLoop or QTest::qWait()
  QCoreApplication app(argc, argv);
  int result = Catch::Session().run(argc, argv);
  return result;
}
