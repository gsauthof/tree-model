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
#include <editor/gui_command/edit.hh>

using namespace std;

TEST_CASE("edit dialog", "[editor][qt][gui][edit]")
{
  QMainWindow w;
  w.show();

  editor::command::Open o;
  editor::gui_command::Edit e(&w);
  o.connect(&o, &editor::command::Open::item_tree_model_created,
      &e, &editor::gui_command::Edit::set_model);
  QAbstractItemModel *model {nullptr};
  o.connect(&o, &editor::command::Open::item_tree_model_created,
      [&model](QAbstractItemModel *m) { model = m; });

  std::string in(test::path::in() + "/small.xml");
  o.open(in.c_str());

  REQUIRE(model != nullptr);

  QTimer::singleShot(300, [&w]{
      // now has explicit focus (everything is selected)
      //QTest::keyClick(w.focusWidget(), Qt::Key_Home, Qt::ShiftModifier, 100);
      QTest::keyClicks(w.focusWidget(), "blah", Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Tab, Qt::NoModifier, 100);
      QTest::keyClicks(w.focusWidget(), "23", Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Tab, Qt::NoModifier, 100);
      QTest::keyClick(w.focusWidget(), Qt::Key_Tab, Qt::NoModifier, 100);
      QTest::keyClick(w.focusWidget(), Qt::Key_Return, Qt::NoModifier, 100);
      });

  auto doc_root = model->index(0, 0, QModelIndex());
  auto foo = model->index(0, 0, doc_root);

  CHECK(model->data(foo).toString().toUtf8().data() == string("foo"));
  e.edit(foo);
  CHECK(model->data(foo).toString().toUtf8().data() == string("blah"));
  CHECK(model->data(foo.sibling(foo.row(), 1)).toString().toUtf8().data() == string("23"));

  delete model;
}

TEST_CASE("edit dialog with children", "[editor][qt][gui][edit]")
{
  QMainWindow w;
  w.show();

  editor::command::Open o;
  editor::gui_command::Edit e(&w);
  o.connect(&o, &editor::command::Open::item_tree_model_created,
      &e, &editor::gui_command::Edit::set_model);
  QAbstractItemModel *model {nullptr};
  o.connect(&o, &editor::command::Open::item_tree_model_created,
      [&model](QAbstractItemModel *m) { model = m; });

  std::string in(test::path::in() + "/small.xml");
  o.open(in.c_str());

  REQUIRE(model != nullptr);

  QTimer::singleShot(300, [&w]{
      //QTest::keyClick(w.focusWidget(), Qt::Key_Home, Qt::ShiftModifier, 100);
      QTest::keyClicks(w.focusWidget(), "void", Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Tab, Qt::NoModifier, 100);
      QTest::keyClick(w.focusWidget(), Qt::Key_Tab, Qt::NoModifier, 100);
      QTest::keyClick(w.focusWidget(), Qt::Key_Return, Qt::NoModifier, 100);
      });

  auto doc_root = model->index(0, 0, QModelIndex());

  CHECK(model->data(doc_root).toString().toUtf8().data() == string("root"));
  e.edit(doc_root);
  CHECK(model->data(doc_root).toString().toUtf8().data() == string("void"));
  CHECK(model->data(doc_root.sibling(doc_root.row(), 1)) == QVariant());

  delete model;
}


TEST_CASE("edit dialog no root", "[editor][qt][gui][edit]")
{
  QMainWindow w;
  w.show();

  editor::command::Open o;
  editor::gui_command::Edit e(&w);
  o.connect(&o, &editor::command::Open::item_tree_model_created,
      &e, &editor::gui_command::Edit::set_model);
  QAbstractItemModel *model {nullptr};
  o.connect(&o, &editor::command::Open::item_tree_model_created,
      [&model](QAbstractItemModel *m) { model = m; });

  std::string in(test::path::in() + "/small.xml");
  o.open(in.c_str());

  REQUIRE(model != nullptr);

  QTimer::singleShot(300, [&w]{
      auto v = QApplication::modalWindow();
      //auto v = QApplication::focusWindow();
      CHECK(!v);
      //CHECK(v->title().toStdString() == "Edit node");
      if (v)
        QTest::keyClick(v, Qt::Key_Escape, Qt::NoModifier, 100);
      });

  e.edit(QModelIndex());
  QTest::qWait(500);

  delete model;
}

