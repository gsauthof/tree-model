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

#include <QEventLoop>
#include <QSignalSpy>
#include <QSlider>

#include <editor/slider_bar.hh>

using namespace std;

TEST_CASE("basic slider bar", "[editor][qt][gui][slider]")
{
  editor::Slider_Bar sb;
  sb.resize(QSize(600, 40));
  sb.show();

  QSignalSpy spy_jump(&sb, SIGNAL(jump_requested(int)));

  // Alternative to QTimer::oneShot + QEventLoop
  // because event loop processing is done during waiting
  QTest::qWait(300);
  sb.init(10, 1000); 
  QTest::qWait(300);

  auto slider = sb.findChild<QSlider*>();
  REQUIRE(slider);
  CHECK(slider->value() == 10);

  auto v = QApplication::focusWindow();
  REQUIRE(v);
  QTest::mouseClick(v, Qt::LeftButton, 0, QPoint(300, 20), 10);
  QTest::qWait(300);

  REQUIRE(spy_jump.size() == 1);

  // gnome shell
  //CHECK(spy_jump.front().front().toInt() > 820);
  // xfwm4
#if (defined(__MINGW32__) || defined(__MINGW64__))
  CHECK(spy_jump.front().front().toInt() > 690);
#else
  CHECK(spy_jump.front().front().toInt() > 765);
#endif
  CHECK(spy_jump.front().front().toInt() < 830);
}
