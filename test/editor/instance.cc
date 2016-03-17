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
#include <editor/main_window.hh>
#include <editor/gui_controller.hh>

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

  bool is_closed = w == nullptr;
  // with Xvfb and xfwm4 the window is still focused, where
  // with Xnest and xfmw4 most of the time it is not
  if (w)
    is_closed = !w->isVisible();

  CHECK(is_closed);

}

TEST_CASE("instance has empty model", "[editor][qt][gui][instance]")
{
  editor::Instance i;

  i.show();

  QTest::qWait(300);
  auto v = QApplication::focusWindow();
  REQUIRE(v);

  QTimer::singleShot(300, []{
      auto v = QApplication::focusWindow();
      REQUIRE(v);
      QTest::keyClick(v, Qt::Key_A,     Qt::AltModifier, 10);
      });


  QTimer::singleShot(600, []{
      auto v = QApplication::focusWindow();
      REQUIRE(v);
      auto w = QApplication::focusWidget();
      REQUIRE(w);
      QTest::keyClicks(w, "foo", Qt::NoModifier, 10);
      QTest::keyClick(v, Qt::Key_Return, Qt::NoModifier, 10);
      });

  QTest::mouseClick(v, Qt::RightButton);

  auto main_window = i.main_window();
  REQUIRE(main_window);
  auto controller = main_window->findChild<editor::Gui_Controller*>();
  REQUIRE(controller);
  auto m = controller->item_tree_model();
  REQUIRE(m);
  CHECK(m->rowCount(QModelIndex()) == 1);
  CHECK(m->data(m->index(0, 0)).toString().toStdString() == "foo");
}

