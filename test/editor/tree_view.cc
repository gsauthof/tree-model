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
#include <QTimer>

#include <editor/tree_view.hh>
#include <editor/command/open_xml.hh>
#include <editor/command/open_ber.hh>
#include <editor/file_type.hh>

using namespace std;

TEST_CASE("tv starts with some expanded", "[editor][gui][tree_view]")
{
  std::string in(test::path::in() + "/tap_3_12_small.xml");

  auto p = editor::command::open_xml(in.c_str());
  REQUIRE(p.first);
  unique_ptr<QAbstractItemModel> mm(p.first);
  auto m = mm.get();

  editor::Tree_View tv;
  // internally, Tree_View calls Tree_View::breadth_first_expand()
  // when a new model is set
  tv.set_model(m);

  CHECK(tv.isExpanded(m->index(0, 0)) == true);
  CHECK(tv.isExpanded(m->index(0, 0).child(0, 0)) == true);
  CHECK(tv.isExpanded(m->index(0, 0).child(1, 0)) == true);
  CHECK(tv.isExpanded(m->index(0, 0).child(2, 0)) == false);
  CHECK(tv.isExpanded(m->index(0, 0).child(3, 0)) == false);
  CHECK(tv.isExpanded(m->index(0, 0).child(4, 0)) == false);

  CHECK(m->data(m->index(0, 0).child(3, 0)).toString().toStdString()
      == "CallEventDetailList");

}

TEST_CASE("tv also provides own cut shortcut", "[editor][gui][tree_view]")
{
  qRegisterMetaType<QModelIndexList>();
  std::string in(test::path::in() + "/tap_3_12_small.xml");

  auto p = editor::command::open_xml(in.c_str());
  REQUIRE(p.first);
  unique_ptr<QAbstractItemModel> mm(p.first);
  auto m = mm.get();

  editor::Tree_View tv;
  QSignalSpy spy_cut(&tv, SIGNAL(cut_triggered(const QModelIndexList &)));

  tv.set_model(m);
  tv.show();
  QTest::qWait(300);

  tv.selectionModel()->select(m->index(0, 0).child(0, 0),
      QItemSelectionModel::Select);
  QTest::keyClick(&tv, Qt::Key_X,  Qt::ControlModifier, 10);

  REQUIRE(spy_cut.size() == 1);
  CHECK(qvariant_cast<QModelIndexList>(spy_cut.front().front()).size() == 1);

}


TEST_CASE("tv localtime in date time edit", "[editor][gui][tree_view][delegate]")
{
  std::string in(test::path::in()
      + "/../../libxfsx/test/in/tap_3_12_valid.ber");

  auto p = editor::command::open_ber(in.c_str());
  REQUIRE(std::get<0>(p));
  unique_ptr<QAbstractItemModel> mm(std::get<0>(p));
  auto m = mm.get();
  editor::Tree_View tv;
  tv.set_model(m);
  tv.resize(QSize(600, 400));
  tv.show();
  QTest::qWait(300);

  CHECK(m->index(0, 0).child(0, 0).child(3, 0).child(0, 1).data().toString().toStdString() == "20050405090547");

  tv.expand(m->index(0, 0).child(0, 0).child(3, 0));
  tv.setCurrentIndex(m->index(0, 0).child(0, 0).child(3, 0).child(0, 1));

  QTest::qWait(300);

  QTest::keyClick(&tv, Qt::Key_F2,  Qt::NoModifier, 10);

  auto v = static_cast<QWidget*>(nullptr);
  QTest::keyClick(v, Qt::Key_Up,  Qt::NoModifier, 10);
  QTest::keyClick(v, Qt::Key_Up,  Qt::NoModifier, 10);
  QTest::keyClick(v, Qt::Key_Up,  Qt::NoModifier, 10);
  QTest::keyClick(v, Qt::Key_Enter,  Qt::NoModifier, 10);
  QTest::qWait(300);

  CHECK(m->index(0, 0).child(0, 0).child(3, 0).child(0, 1).data().toString().toStdString() == "20080405090547");

  QTest::qWait(300);
}


TEST_CASE("tv complete sender", "[editor][gui][tree_view][delegate]")
{
  std::string in(test::path::in()
      + "/../../libxfsx/test/in/tap_3_12_valid.ber");

  editor::File_Type ft;
  auto p = editor::command::open_ber(in.c_str(), ft);
  REQUIRE(std::get<0>(p));
  unique_ptr<QAbstractItemModel> mm(std::get<0>(p));
  auto m = mm.get();
  editor::Tree_View tv;
  tv.set_model(m);
  tv.apply_file_type(ft);
  tv.resize(QSize(600, 400));
  tv.show();
  QTest::qWait(300);

  CHECK(m->index(0, 0).child(0, 0).child(1, 1).data().toString().toStdString() == "XLKJE");

  tv.setCurrentIndex(m->index(0, 0).child(0, 0).child(1, 1));

  QTest::qWait(300);

  QTest::keyClick(&tv, Qt::Key_F2,  Qt::NoModifier, 10);

  auto v = static_cast<QWidget*>(nullptr);
  QTest::keyClicks(v, "USA");
  QTest::keyClick(v, Qt::Key_Down,  Qt::NoModifier, 10);
  QTest::keyClick(v, Qt::Key_Down,  Qt::NoModifier, 10);
  QTest::keyClick(v, Qt::Key_Down,  Qt::NoModifier, 10);
  QTest::keyClick(v, Qt::Key_Down,  Qt::NoModifier, 900);

  // Work-around keyClick() fragility in CI environment
  // keyClick() internally calls Press/Release - and aparently,
  // sub-widget gets destroyed in between ...
  QTest::keyPress(v, Qt::Key_Enter,  Qt::NoModifier, 10);
  QTest::qWait(300);
  QTest::keyRelease(v, Qt::Key_Enter,  Qt::NoModifier, 10);
  QTest::qWait(300);
  QTest::keyPress(v, Qt::Key_Enter,  Qt::NoModifier, 10);
  QTest::qWait(300);
  QTest::keyRelease(v, Qt::Key_Enter,  Qt::NoModifier, 10);

  CHECK(m->index(0, 0).child(0, 0).child(1, 1).data().toString().toStdString() == "USAAT");
  //CHECK(m->index(0, 0).child(0, 0).child(1, 1).data().toString().toStdString() == "USA31");

  QTest::qWait(300);
}
