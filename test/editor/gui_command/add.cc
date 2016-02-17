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

#include <QAbstractItemModel>
#include <QMainWindow>
#include <string>

#include <editor/command/open.hh>
#include <editor/gui_command/add.hh>

using namespace std;

TEST_CASE("add dialog", "[editor][qt][gui][add]")
{
  QMainWindow w;
  w.show();

  editor::command::Open o;
  editor::gui_command::Add a(&w);
  o.connect(&o, &editor::command::Open::item_tree_model_created,
      &a, &editor::gui_command::Add::set_model);
  QAbstractItemModel *model {nullptr};
  o.connect(&o, &editor::command::Open::item_tree_model_created,
      [&model](QAbstractItemModel *m) { model = m; });

  std::string in(test::path::in() + "/small.xml");
  o.open(in.c_str());

  REQUIRE(model != nullptr);

  QTimer::singleShot(300, [&w]{
      QTest::keyClick(w.focusWidget(), Qt::Key_Home, Qt::ShiftModifier, 100);
      QTest::keyClicks(w.focusWidget(), "blah", Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Tab, Qt::NoModifier, 100);
      QTest::keyClicks(w.focusWidget(), "23", Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Tab, Qt::NoModifier, 100);
      QTest::keyClick(w.focusWidget(), Qt::Key_Tab, Qt::NoModifier, 100);
      QTest::keyClick(w.focusWidget(), Qt::Key_Return, Qt::NoModifier, 100);
      });

  auto doc_root = model->index(0, 0, QModelIndex());

  CHECK(model->rowCount(doc_root) == 2);
  a.add_child(doc_root);
  REQUIRE(model->rowCount(doc_root) == 3);
  auto x = model->index(2, 0, doc_root);
  CHECK(model->data(x).toString().toUtf8().data() == string("blah"));
  CHECK(model->data(x.sibling(x.row(), 1)).toString().toUtf8().data() == string("23"));

  delete model;
}

TEST_CASE("add sibling dialog", "[editor][qt][gui][add]")
{
  QMainWindow w;
  w.show();

  editor::command::Open o;
  editor::gui_command::Add a(&w);
  o.connect(&o, &editor::command::Open::item_tree_model_created,
      &a, &editor::gui_command::Add::set_model);
  QAbstractItemModel *model {nullptr};
  o.connect(&o, &editor::command::Open::item_tree_model_created,
      [&model](QAbstractItemModel *m) { model = m; });

  std::string in(test::path::in() + "/small.xml");
  o.open(in.c_str());

  REQUIRE(model != nullptr);

  QTimer::singleShot(300, [&w]{
      QTest::keyClick(w.focusWidget(), Qt::Key_Home, Qt::ShiftModifier, 100);
      QTest::keyClicks(w.focusWidget(), "blah", Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Tab, Qt::NoModifier, 100);
      QTest::keyClicks(w.focusWidget(), "23", Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Tab, Qt::NoModifier, 100);
      QTest::keyClick(w.focusWidget(), Qt::Key_Tab, Qt::NoModifier, 100);
      QTest::keyClick(w.focusWidget(), Qt::Key_Return, Qt::NoModifier, 100);
      });

  auto doc_root = model->index(0, 0);

  CHECK(model->rowCount(doc_root) == 2);
  a.add_sibling(doc_root.child(1,0)); // -> opens dialog + event loop

  REQUIRE(model->rowCount(doc_root) == 3);
  auto x = model->index(1, 0, doc_root);
  CHECK(model->data(x).toString().toStdString() == "blah");
  CHECK(model->data(x.sibling(x.row(), 1)).toString().toStdString() == "23");
  x = model->index(0, 0, doc_root);
  CHECK(model->data(x).toString().toStdString() == "foo");
  CHECK(model->data(x.sibling(x.row(), 1)).toString().toStdString() == "Hello");

  delete model;
}

TEST_CASE("add child in keyvalue fail", "[editor][qt][gui][add]")
{
  QMainWindow w;
  w.show();

  editor::command::Open o;
  editor::gui_command::Add a(&w);
  o.connect(&o, &editor::command::Open::item_tree_model_created,
      &a, &editor::gui_command::Add::set_model);
  QAbstractItemModel *model {nullptr};
  o.connect(&o, &editor::command::Open::item_tree_model_created,
      [&model](QAbstractItemModel *m) { model = m; });

  std::string in(test::path::in() + "/small.xml");
  o.open(in.c_str());

  REQUIRE(model != nullptr);

  QTimer::singleShot(300, [&w]{
      auto v = QApplication::modalWindow();
      CHECK(!v);
      //CHECK(v->title().toStdString() != "Add child");
      });

  auto point = model->index(0, 0).child(0, 0);

  CHECK(model->rowCount(point) == 0);
  a.add_child(point); // -> should not open dialog

  REQUIRE(model->rowCount(point) == 0);

  auto x = model->index(0, 0).child(0, 0);
  CHECK(model->data(x).toString().toStdString() == "foo");
  CHECK(model->data(x.sibling(x.row(), 1)).toString().toStdString() == "Hello");

  delete model;
}
