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

#include <boost/filesystem.hpp>

#include <editor/instance_manager.hh>

TEST_CASE("instance manager basic", "[editor][qt][gui][instance][manager]")
{
  editor::Instance_Manager i;

  QTest::qWait(300);

  auto w = QApplication::focusWindow();
  REQUIRE(w);
  CHECK(w->title().toStdString() == "unnamed");

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  i.first_open_requested(in.c_str());

  QTest::qWait(300);

  CHECK(w->title().endsWith("tap_3_12_small.xml"));

  i.quit();
  QTest::qWait(300);

  w = QApplication::focusWindow();
  CHECK(!w);

}

TEST_CASE("instance manager two", "[editor][qt][gui][instance][manager]")
{
  editor::Instance_Manager i;

  QTest::qWait(300);

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  i.first_open_requested(in.c_str());

  QTest::qWait(300);

  auto w = QApplication::focusWindow();
  REQUIRE(w);
  CHECK(w->title().endsWith("tap_3_12_small.xml"));
  CHECK(QApplication::topLevelWindows().size() == 1);

  i.do_create_instance();
  QTest::qWait(300);

  std::string in2(test::path::in() + "/small.xml");
  QTest::qWait(300);
  w = QApplication::focusWindow();
  CHECK(w->title().endsWith("unnamed"));
  CHECK(QApplication::topLevelWindows().size() == 2);

  i.quit();
  QTest::qWait(300);

  w = QApplication::focusWindow();
  if (w) {
    for (auto w : QApplication::topLevelWindows())
      CHECK(w->isVisible() == false);
  }
}

TEST_CASE("instance manager two shortcut", "[editor][qt][gui][instance][manager]")
{
  editor::Instance_Manager i;

  QTest::qWait(300);

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  i.first_open_requested(in.c_str());

  QTest::qWait(300);

  auto w = QApplication::focusWindow();
  REQUIRE(w);
  CHECK(w->title().endsWith("tap_3_12_small.xml"));
  CHECK(QApplication::topLevelWindows().size() == 1);

  QTest::keyClick(w, Qt::Key_N, Qt::ControlModifier, 10);
  QTest::qWait(300);

  w = QApplication::focusWindow();
  CHECK(w->title().endsWith("unnamed"));
  CHECK(QApplication::topLevelWindows().size() == 2);

  i.quit();
  QTest::qWait(300);

  w = QApplication::focusWindow();
  if (w) {
    for (auto w : QApplication::topLevelWindows())
      CHECK(w->isVisible() == false);
  }
}

TEST_CASE("im tree close ask discard", "[editor][qt][gui][instance][manager]")
{
  editor::Instance_Manager i;

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  auto old_size = boost::filesystem::file_size(in);
  i.first_open_requested(in.c_str());

  QTest::qWait(300);

  auto v = QApplication::focusWindow();
  QTest::keyClick(v, Qt::Key_Right, Qt::NoModifier, 10);
  QTest::keyClick(v, Qt::Key_Down,  Qt::NoModifier, 10);
  QTest::keyClick(v, Qt::Key_Delete,Qt::NoModifier, 10);

  QTest::qWait(100);

  QTimer::singleShot(300, []() {
    auto v = QApplication::modalWindow();
    REQUIRE(v);
    CHECK(v->title().toStdString() == "");

    // default button is discard -> return triggers discard
    QTest::keyClick(v, Qt::Key_Return, Qt::NoModifier, 10);
    QTest::qWait(300);
  });
  QTest::keyClick(v, Qt::Key_Q, Qt::ControlModifier, 10);
  QTest::qWait(500);

  for (auto v : QApplication::topLevelWindows()) {
    CHECK(v->isVisible() == false);
  }
  auto new_size = boost::filesystem::file_size(in);
  // test that it wasn't saved
  CHECK(old_size == new_size);
}
