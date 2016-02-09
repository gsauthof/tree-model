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
#include <catch.hpp>
#include <test/test.hh>

#include <QtTest/QtTest>
#include <QDebug>
#include <QApplication>

#include <editor/instance.hh>

TEST_CASE("instance basic", "[editor][qt][gui][instance]")
{
  editor::Instance i;

  i.show();

  QTest::qWait(300);

  auto w = QApplication::focusWindow();
  REQUIRE(w);
  CHECK(w->title().toStdString() == "unnamed");

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  i.open(in.c_str());

  QTest::qWait(300);

  CHECK(w->title().endsWith("tap_3_12_small.xml"));

  i.close();
  QTest::qWait(300);

  w = QApplication::focusWindow();
  CHECK(!w);

}
