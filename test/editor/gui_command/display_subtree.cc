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
#include <QApplication>
#include <QSignalSpy>
#include <QMainWindow>

#include <editor/command/open_xml.hh>
#include <editor/gui_command/display_subtree.hh>
#include <editor/subtree_window.hh>
#include <editor/tree_widget.hh>
#include <editor/tree_view.hh>
#include <tree_model/item_adaptor.hh>
#include <tree_model/xml.hh>

using namespace std;

TEST_CASE("basic display subtree", "[editor][qt][gui][subtree]")
{
  tree_model::XML *m = new tree_model::XML(xxxml::read_file(
        test::path::in() + "/tap_3_12_small.xml"));
  tree_model::Item_Adaptor a(m);

  QMainWindow w;
  editor::gui_command::Display_Subtree ds(&w);

  qRegisterMetaType<editor::Subtree_Window*>();
  QSignalSpy spy_created(&ds,
      SIGNAL(subtree_window_created(editor::Subtree_Window*)));

  ds.set_model(&a);
  auto sm = new QItemSelectionModel(&a);
  ds.set_selection_model(sm);
  sm->select(a.index(0, 0).child(3, 0).child(1, 0),
    QItemSelectionModel::Select);

  ds.display();

  QTimer::singleShot(300, [&w, &spy_created, &a]{
    auto v = QApplication::focusWindow();
    REQUIRE(v != nullptr);
    CHECK(v->title().toStdString() == "MobileOriginatedCall #2");

    REQUIRE(spy_created.empty() == false);
    auto x = qvariant_cast<editor::Subtree_Window*>(
        spy_created.front().front());
    REQUIRE(x != nullptr);
    auto tv = &x->tree_widget().tree_view();
    CHECK(tv->rootIndex() == a.index(0, 0).child(3, 0).child(1, 0));
    CHECK(tv->isExpanded(tv->rootIndex().child(0, 0)) == true);
    CHECK(tv->isExpanded(tv->rootIndex().child(0, 0).child(0, 0)) == true);
    CHECK(tv->isExpanded(tv->rootIndex().child(1, 0)) == true);
    CHECK(tv->isExpanded(tv->rootIndex().child(1, 0).child(0,0)) == true);
    });

  QEventLoop e;
  QTimer::singleShot(1000, [&e]{ e.quit(); });
  e.exec();

}

TEST_CASE("subtree has undo redo", "[editor][gui][subtree]")
{
  std::string in(test::path::in() + "/tap_3_12_small.xml");

  auto p = editor::command::open_xml(in.c_str());
  REQUIRE(p.first);
  unique_ptr<QAbstractItemModel> mm(p.first);
  auto m = mm.get();

  editor::gui_command::Display_Subtree ds;
  QSignalSpy spy_undo(&ds, SIGNAL(undo_requested()));
  QSignalSpy spy_redo(&ds, SIGNAL(redo_requested()));

  ds.set_model(m);
  auto sm = new QItemSelectionModel(m);
  ds.set_selection_model(sm);
  sm->select(m->index(0, 0), QItemSelectionModel::Select);

  editor::Subtree_Window *w = nullptr;
  QObject::connect(&ds,
      &editor::gui_command::Display_Subtree::subtree_window_created,
      [&w](auto x) { w = x; });

  ds.display();
  QTest::qWait(300);

  REQUIRE(w);
  w->show();
  QTest::qWait(300);

  auto tw = w->findChild<editor::Tree_Widget*>();
  REQUIRE(tw);
  auto tv = tw->findChild<editor::Tree_View*>();
  REQUIRE(tv);

  QTest::keyClick(tv, Qt::Key_Z,  Qt::ControlModifier, 10);
  QTest::qWait(300);
  CHECK(spy_undo.size() == 1);

  QTest::keyClick(tv, Qt::Key_Z,  Qt::ShiftModifier | Qt::ControlModifier, 10);
  QTest::qWait(300);

  CHECK(spy_redo.size() == 1);

}
