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

#include <editor/gui_command/search.hh>
#include <editor/command/open_xml.hh>
#include <editor/result_window.hh>
#include <editor/table_view.hh>

#include <QMainWindow>
#include <QApplication>
#include <QSignalSpy>

#include <string>

using namespace std;

TEST_CASE("gui search", "[editor][search][gui]")
{
  QMainWindow w;
  w.show();

  string in {test::path::in() + "/tap_3_12_small.xml"};
  auto r = editor::command::open_xml(in.c_str());
  unique_ptr<QAbstractItemModel> m(r.first);


  editor::gui_command::Search search(&w);
  search.set_model(r.first);
  search.set_tree_model(r.second);
  QSignalSpy spy_current(&search, SIGNAL(current_changed(const QModelIndex&,
          const QModelIndex &)));

  QTimer::singleShot(300, [](){
      QWidget *w = nullptr;
      auto v = QApplication::focusWindow();
      QTest::keyClicks(w, "Imsi",  Qt::NoModifier, 10);
      QTest::keyClick(v, Qt::Key_Tab,  Qt::NoModifier, 10);
      QTest::keyClicks(w, "133713371337133",  Qt::NoModifier, 10);
      QTest::keyClick(w, Qt::Key_Return,  Qt::NoModifier, 10);
      });
  search.display();
  QTest::qWait(300);
  auto rw = w.findChild<editor::Result_Window*>();
  REQUIRE(rw);
  auto tv = rw->findChild<editor::Table_View*>();
  REQUIRE(tv);
  auto tm = tv->model();
  REQUIRE(tm);
  CHECK(tm->rowCount() == 2);
  CHECK(tm->index(0, 0).data().toString().toStdString() == "Imsi");
  CHECK(tm->index(0, 1).data().toString().toStdString() == "133713371337133");

  tv->setCurrentIndex(tm->index(1, 0));

  REQUIRE(spy_current.size() <= 2);

  auto i = spy_current.back().front().toModelIndex();
  CHECK(i.isValid());
  CHECK(i.model() == m.get());
  CHECK(i == m->index(0, 0).child(3, 0)
      .child(3, 0).child(0, 0).child(0, 0).child(0, 0));
}


TEST_CASE("gui search empty", "[editor][search][gui]")
{
  QMainWindow w;
  w.show();

  string in {test::path::in() + "/tap_3_12_small.xml"};
  auto r = editor::command::open_xml(in.c_str());
  unique_ptr<QAbstractItemModel> m(r.first);


  editor::gui_command::Search search(&w);
  search.set_model(r.first);
  search.set_tree_model(r.second);
  QSignalSpy spy_current(&search, SIGNAL(current_changed(const QModelIndex&,
          const QModelIndex &)));

  QTimer::singleShot(300, [](){
      QWidget *w = nullptr;
      auto v = QApplication::focusWindow();
      QTest::keyClicks(w, "Imsix",  Qt::NoModifier, 10);
      QTest::keyClick(v, Qt::Key_Tab,  Qt::NoModifier, 10);
      QTest::keyClicks(w, "133713371337133",  Qt::NoModifier, 10);
      QTest::keyClick(w, Qt::Key_Return,  Qt::NoModifier, 10);
      });
  search.display();
  QTest::qWait(300);
  auto rw = w.findChild<editor::Result_Window*>();
  REQUIRE(!rw);
}
