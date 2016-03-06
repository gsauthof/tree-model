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
#include <QClipboard>
#include <QEventLoop>
#include <QDialog>
#include <QSettings>
#include <QToolBar>
#include <QAction>
#include <string>

#include <editor/gui_controller.hh>
#include <editor/main_window.hh>
#include <editor/vc_matchmaker.hh>
#include <editor/subtree_window.hh>
#include <editor/tree_widget.hh>
#include <editor/tree_view.hh>

using namespace std;

TEST_CASE("copy action", "[editor][qt][gui][mainwindow][clipboard]")
{
  editor::Main_Window w;
  editor::Gui_Controller c(&w);
  editor::connect_view_controller(w, c);

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  QTimer::singleShot(0, [&c, &in]() { c.open(in.c_str()); });

  QApplication::clipboard()->clear();

  int old_rowcount = 0;
  QAbstractItemModel *a = nullptr;

  QTimer::singleShot(300, [&w, &old_rowcount, &a, &c]{
      a = c.item_tree_model();
      REQUIRE(a != nullptr);
      old_rowcount = a->rowCount(a->index(0, 0, QModelIndex()).child(0, 0));

      QTest::keyClick(w.focusWidget(), Qt::Key_Right, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Right, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down, Qt::NoModifier, 10);
      // TODO, make this portable, e.g. via something like QKeySequence::Copy ?
      QTest::keyClick(w.focusWidget(), Qt::Key_C, Qt::ControlModifier, 10);
      });

  w.show();

  QEventLoop e;
  QTimer::singleShot(1000, [&e]{ e.quit(); });
  e.exec();

  const char ref[] = R"(<TransferCutOffTimeStamp>
            <LocalTimeStamp>20050405090547</LocalTimeStamp>
            <UtcTimeOffset>+0200</UtcTimeOffset>
        </TransferCutOffTimeStamp>)";

  CHECK(QApplication::clipboard()->text().toStdString() == ref);
  auto new_rowcount = a->rowCount(a->index(0, 0, QModelIndex()).child(0, 0));
  CHECK(old_rowcount == new_rowcount);

  QApplication::clipboard()->clear();
}

TEST_CASE("cut action", "[editor][qt][gui][mainwindow][clipboard]")
{
  editor::Main_Window w;
  editor::Gui_Controller c(&w);
  editor::connect_view_controller(w, c);

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  QTimer::singleShot(0, [&c, &in]() { c.open(in.c_str()); });

  QApplication::clipboard()->clear();

  int old_rowcount = 0;
  QAbstractItemModel *a = nullptr;

  QTimer::singleShot(300, [&w, &old_rowcount, &a, &c]{
      a = c.item_tree_model();
      REQUIRE(a != nullptr);
      old_rowcount = a->rowCount(a->index(0, 0, QModelIndex()).child(0, 0));

      QTest::keyClick(w.focusWidget(), Qt::Key_Right, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Right, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down, Qt::NoModifier, 10);
      // TODO, make this portable, e.g. via something like QKeySequence::Cut ?
      QTest::keyClick(w.focusWidget(), Qt::Key_X, Qt::ControlModifier, 10);
      });

  w.show();

  QEventLoop e;
  QTimer::singleShot(1000, [&e]{ e.quit(); });
  e.exec();

  const char ref[] = R"(<TransferCutOffTimeStamp>
            <LocalTimeStamp>20050405090547</LocalTimeStamp>
            <UtcTimeOffset>+0200</UtcTimeOffset>
        </TransferCutOffTimeStamp>)";

  CHECK(QApplication::clipboard()->text().toStdString() == ref);
  auto new_rowcount = a->rowCount(a->index(0, 0, QModelIndex()).child(0, 0));
  CHECK(old_rowcount == new_rowcount + 1);

  QApplication::clipboard()->clear();
}

TEST_CASE("delete shortcut", "[editor][qt][gui][mainwindow]")
{
  editor::Main_Window w;
  editor::Gui_Controller c(&w);
  editor::connect_view_controller(w, c);

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  QTimer::singleShot(0, [&c, &in]() { c.open(in.c_str()); });

  int old_rowcount = 0;
  QAbstractItemModel *a = nullptr;

  QTimer::singleShot(300, [&w, &old_rowcount, &a, &c]{
      a = c.item_tree_model();
      REQUIRE(a != nullptr);
      old_rowcount = a->rowCount(a->index(0, 0, QModelIndex()).child(1, 0));

      // collapse some
      QTest::keyClick(w.focusWidget(), Qt::Key_E,  Qt::ShiftModifier | Qt::ControlModifier, 10);

      // 2nd right necessary for xfwm4
      QTest::keyClick(w.focusWidget(), Qt::Key_Right,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Right,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,   Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,   Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Right,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,   Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,   Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,   Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Delete, Qt::NoModifier, 10);
      });

  w.show();

  QEventLoop e;
  QTimer::singleShot(1000, [&e]{ e.quit(); });
  e.exec();

  auto new_rowcount = a->rowCount(a->index(0, 0, QModelIndex()).child(1, 0));
  CHECK(old_rowcount == new_rowcount + 1);

}

TEST_CASE("paste shortcut", "[editor][qt][gui][mainwindow]")
{
  editor::Main_Window w;
  editor::Gui_Controller c(&w);
  editor::connect_view_controller(w, c);

  QClipboard *cb = QApplication::clipboard();
  cb->clear();
  auto d = new QMimeData();
  d->setData("text/xml", QByteArray("<OperatorSpecInfoList>"
        "<OperatorSpecInformation>Foo</OperatorSpecInformation>"
        "</OperatorSpecInfoList>"));
  cb->setMimeData(d);

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  QTimer::singleShot(0, [&c, &in]() { c.open(in.c_str()); });

  int old_rowcount = 0;
  QAbstractItemModel *a = nullptr;

  QTimer::singleShot(300, [&w, &old_rowcount, &a, &c]{
      a = c.item_tree_model();
      REQUIRE(a != nullptr);
      old_rowcount = a->rowCount(a->index(0, 0, QModelIndex()).child(1, 0));

      // collapse some
      QTest::keyClick(w.focusWidget(), Qt::Key_E,  Qt::ShiftModifier | Qt::ControlModifier, 10);

      // 2nd right necessary for xfwm4
      QTest::keyClick(w.focusWidget(), Qt::Key_Right,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Right,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,   Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,   Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Right,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_V, Qt::ControlModifier | Qt::ShiftModifier, 10);
      });

  w.show();

  QEventLoop e;
  QTimer::singleShot(1000, [&e]{ e.quit(); });
  e.exec();

  auto new_rowcount = a->rowCount(a->index(0, 0, QModelIndex()).child(1, 0));
  CHECK(old_rowcount + 1 == new_rowcount);

  cb->clear();
}

TEST_CASE("paste child shortcut", "[editor][qt][gui][mainwindow]")
{
  editor::Main_Window w;
  editor::Gui_Controller c(&w);
  editor::connect_view_controller(w, c);

  QClipboard *cb = QApplication::clipboard();
  cb->clear();
  auto d = new QMimeData();
  d->setData("text/xml", QByteArray("<Sender>GMS23</Sender>"));
  cb->setMimeData(d);

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  QTimer::singleShot(0, [&c, &in]() { c.open(in.c_str()); });

  int old_rowcount = 0;
  QAbstractItemModel *a = nullptr;

  QTimer::singleShot(300, [&w, &old_rowcount, &a, &c]{
      a = c.item_tree_model();
      REQUIRE(a != nullptr);
      old_rowcount = a->rowCount(a->index(0, 0, QModelIndex()).child(0, 0));

      QTest::keyClick(w.focusWidget(), Qt::Key_Right, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Right, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_V,     Qt::ControlModifier, 10);
      });

  w.show();

  QEventLoop e;
  QTimer::singleShot(1000, [&e]{ e.quit(); });
  e.exec();

  auto new_rowcount = a->rowCount(a->index(0, 0, QModelIndex()).child(0, 0));
  CHECK(old_rowcount + 1 == new_rowcount);
  CHECK(a->data(a->index(0,0).child(0, 0).child(0, 1))
      .toString().toStdString() == "GMS23");
  CHECK(a->data(a->index(0,0).child(0, 0).child(1, 1))
      .toString().toStdString() == "WERFD");

  cb->clear();
}

TEST_CASE("mw display subtree", "[editor][qt][gui][mainwindow]")
{
  editor::Main_Window w;
  editor::Gui_Controller c(&w);
  editor::connect_view_controller(w, c);

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  QTimer::singleShot(0, [&c, &in]() { c.open(in.c_str()); });

  QAbstractItemModel *a = nullptr;

  QTimer::singleShot(300, [&w, &a, &c]{
      a = c.item_tree_model();
      REQUIRE(a != nullptr);

      // collapse some
      QTest::keyClick(w.focusWidget(), Qt::Key_E,  Qt::ShiftModifier | Qt::ControlModifier, 10);

      // 2nd right necessary for xfwm4
      QTest::keyClick(w.focusWidget(), Qt::Key_Right, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Right, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Right, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_F5,    Qt::NoModifier, 10);
      });

  QTimer::singleShot(900, [&w, &a, &c]{
    auto v = w.findChild<QDialog*>();
    REQUIRE(v);
    CHECK(v->windowTitle().toStdString() == "GprsCall #3");
    });

  w.show();

  QEventLoop e;
  QTimer::singleShot(2000, [&e]{ e.quit(); });
  e.exec();

}


TEST_CASE("mw display subtree model change", "[editor][qt][gui][mainwindow]")
{
  editor::Main_Window w;
  editor::Gui_Controller c(&w);
  editor::connect_view_controller(w, c);

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  QTimer::singleShot(0, [&c, &in]() { c.open(in.c_str()); });
  std::string in2(test::path::in() + "/small.xml");

  QAbstractItemModel *a = nullptr;

  QTimer::singleShot(300, [&w, &a, &c]{
      a = c.item_tree_model();
      REQUIRE(a != nullptr);

      // collapse some
      QTest::keyClick(w.focusWidget(), Qt::Key_E,  Qt::ShiftModifier | Qt::ControlModifier, 10);

      // 2nd right necessary for xfwm4
      QTest::keyClick(w.focusWidget(), Qt::Key_Right, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Right, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Right, Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_Down,  Qt::NoModifier, 10);
      QTest::keyClick(w.focusWidget(), Qt::Key_F5,    Qt::NoModifier, 10);
      });

  QTimer::singleShot(900, [&w, &a, &c, &in2]{
    auto v = w.findChild<QDialog*>();
    REQUIRE(v);
    CHECK(v->windowTitle().toStdString() == "GprsCall #3");
      c.open(in2.c_str());
    });

  QTimer::singleShot(1900, [&w, &a, &c]{
    auto v = w.findChild<editor::Subtree_Window*>();
    REQUIRE(v);
    CHECK(v->tree_widget().tree_view().rootIndex().isValid() == false);
    });

  w.show();

  QEventLoop e;
  QTimer::singleShot(3000, [&e]{ e.quit(); });
  e.exec();

}

TEST_CASE("mw tree view context menu", "[editor][qt][gui][mainwindow]")
{
  editor::Main_Window w;
  editor::Gui_Controller c(&w);
  editor::connect_view_controller(w, c);

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  QTimer::singleShot(0, [&c, &in]() { c.open(in.c_str()); });

  QAbstractItemModel *a = nullptr;
  int old_rowcount = 0;
  QTimer::singleShot(300, [&w, &c, &a, &old_rowcount]{
      a = c.item_tree_model();
      REQUIRE(a != nullptr);
      old_rowcount = a->rowCount(a->index(0, 0));
      auto v = w.focusWidget();
      REQUIRE(v);

      // collapse some
      QTest::keyClick(v, Qt::Key_E,  Qt::ShiftModifier | Qt::ControlModifier, 10);

      // 2nd right necessary for xfwm4
      QTest::keyClick(v, Qt::Key_Right, Qt::NoModifier, 10);
      QTest::keyClick(v, Qt::Key_Right, Qt::NoModifier, 10);
      QTest::keyClick(v, Qt::Key_Down,  Qt::NoModifier, 10);
      QTest::keyClick(v, Qt::Key_Down,  Qt::NoModifier, 10);
      });
  QTimer::singleShot(600, [&w]{
      auto v = QApplication::focusWindow();
      REQUIRE(v);
      QTest::mouseClick(v, Qt::RightButton);
      });
  QTimer::singleShot(900, [&w]{
      auto v = QApplication::focusWindow();
      REQUIRE(v);
      // remove action
      QTest::keyClick(v, Qt::Key_M, Qt::AltModifier, 10);
      });
  int new_rowcount = 0;
  QTimer::singleShot(1200, [&w, &a, &old_rowcount, &new_rowcount]{
      new_rowcount = a->rowCount(a->index(0, 0));
      CHECK(old_rowcount == new_rowcount + 1);
      });

  w.show();

  QEventLoop e;
  QTimer::singleShot(3000, [&e]{ e.quit(); });
  e.exec();
}

TEST_CASE("mw tree view slider goto", "[editor][qt][gui][mainwindow]")
{
  editor::Main_Window w;
  editor::Gui_Controller c(&w);
  editor::connect_view_controller(w, c);

  QClipboard *cb = QApplication::clipboard();
  cb->clear();

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  QTimer::singleShot(0, [&c, &in]() { c.open(in.c_str()); });

  QAbstractItemModel *a = nullptr;
  int old_rowcount = 0;
  QTimer::singleShot(300, [&w, &c, &a, &old_rowcount]{
      a = c.item_tree_model();
      REQUIRE(a != nullptr);
      auto v = QApplication::focusWindow();
      REQUIRE(v != nullptr);
      QTest::keyClick(v, Qt::Key_Right, Qt::NoModifier, 10);
      QTest::keyClick(v, Qt::Key_Down,  Qt::NoModifier, 10);
      QTest::keyClick(v, Qt::Key_Right, Qt::NoModifier, 10);
      QTest::keyClick(v, Qt::Key_Down,  Qt::NoModifier, 10);
      QTest::keyClick(v, Qt::Key_G,     Qt::ControlModifier, 10);
      // -> in rank line edit
      QTest::keyClick(v, Qt::Key_3,     Qt::NoModifier, 10);
      QTest::keyClick(v, Qt::Key_Return,Qt::NoModifier, 10);
      QTest::keyClick(v, Qt::Key_Tab,   Qt::NoModifier, 10);
      QTest::keyClick(v, Qt::Key_Tab,   Qt::NoModifier, 10);
      // -> in tree_view
      QTest::keyClick(v, Qt::Key_Up,    Qt::NoModifier, 10);
      // -> make sure that current index is also correctly set
      QTest::keyClick(v, Qt::Key_C,     Qt::ControlModifier, 10);
      });

  w.show();

  QEventLoop e;
  QTimer::singleShot(1000, [&e]{ e.quit(); });
  e.exec();

  CHECK(cb->mimeData()->data("text/xml").data()
      == string("<Recipient>XLKJE</Recipient>"));
  cb->clear();
}

TEST_CASE("mw tree close ask cancel", "[editor][qt][gui][mainwindow]")
{
  editor::Main_Window w;
  editor::Gui_Controller c(&w);
  editor::connect_view_controller(w, c);

  w.show();

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  c.open(in.c_str());

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

    QTest::keyClick(v, Qt::Key_Escape, Qt::NoModifier, 10);
    QTest::qWait(300);
  });
  QTest::keyClick(v, Qt::Key_W, Qt::ControlModifier, 10);
  QTest::qWait(500);

  REQUIRE(QApplication::topLevelWindows().size() == 1);
  v = QApplication::topLevelWindows().front();
  // does not fly for focus-follows-mouse window managers ...
  //v = QApplication::focusWindow();
  REQUIRE(v);
  CHECK(v->title().endsWith("tap_3_12_small.xml*"));
  QTest::qWait(300);
}

TEST_CASE("mw tree view add sibling", "[editor][qt][gui][mainwindow]")
{
  editor::Main_Window w;
  editor::Gui_Controller c(&w);
  editor::connect_view_controller(w, c);

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  c.open(in.c_str());
  QTest::qWait(300);

  w.show();
  QTest::qWait(100);
  QAbstractItemModel *a = c.item_tree_model();
  REQUIRE(a != nullptr);
  int old_rowcount = a->rowCount(a->index(0, 0));
  auto v = QApplication::focusWindow();
  REQUIRE(v != nullptr);

  // collapse some
  QTest::keyClick(v, Qt::Key_E,  Qt::ShiftModifier | Qt::ControlModifier, 10);

  QTest::keyClick(v, Qt::Key_Right, Qt::NoModifier, 10);
  QTest::keyClick(v, Qt::Key_Down,  Qt::NoModifier, 10);
  QTest::keyClick(v, Qt::Key_Down,  Qt::NoModifier, 10);

  QTimer::singleShot(300, [&v]{
      auto v = QApplication::focusWindow();
      REQUIRE(v);
      QTest::keyClick(v, Qt::Key_S, Qt::AltModifier, 10);
      });

  QTimer::singleShot(600, [&w]{
      auto v = QApplication::modalWindow();
      REQUIRE(v);
      CHECK(v->title().toStdString() == "Add sibling");
      QTest::keyClick(v, Qt::Key_Home, Qt::ShiftModifier, 10);
      QTest::keyClicks(QApplication::focusWidget(), "blah", Qt::NoModifier, 10);
      QTest::keyClick(v, Qt::Key_Tab, Qt::NoModifier, 10);
      QTest::keyClicks(QApplication::focusWidget(), "23", Qt::NoModifier, 10);
      QTest::keyClick(v, Qt::Key_O, Qt::AltModifier, 10);
      });

  QTest::keyClick(v, Qt::Key_E, Qt::AltModifier, 10);
  QTest::qWait(1000);

  int new_rowcount = a->rowCount(a->index(0, 0));
  CHECK(old_rowcount + 1 == new_rowcount);
}

TEST_CASE("mw tree view remove via menu", "[editor][qt][gui][mainwindow]")
{
  editor::Main_Window w;
  editor::Gui_Controller c(&w);
  editor::connect_view_controller(w, c);

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  c.open(in.c_str());
  QTest::qWait(300);

  w.show();
  QTest::qWait(100);
  QAbstractItemModel *a = c.item_tree_model();
  REQUIRE(a != nullptr);
  int old_rowcount = a->rowCount(a->index(0, 0));
  auto v = QApplication::focusWindow();
  REQUIRE(v != nullptr);

  // collapse some
  QTest::keyClick(v, Qt::Key_E,  Qt::ShiftModifier | Qt::ControlModifier, 10);

  QTest::keyClick(v, Qt::Key_Right, Qt::NoModifier, 10);
  QTest::keyClick(v, Qt::Key_Down,  Qt::NoModifier, 10);
  QTest::keyClick(v, Qt::Key_Down,  Qt::NoModifier, 10);
  QTest::keyClick(v, Qt::Key_E,     Qt::AltModifier, 10);
  QTest::qWait(100);
  QTest::keyClick(v, Qt::Key_M,     Qt::AltModifier, 10);
  QTest::qWait(100);

  int new_rowcount = a->rowCount(a->index(0, 0));
  CHECK(old_rowcount == new_rowcount + 1);
}


TEST_CASE("mw tree view add child undo clean", "[editor][qt][gui][mainwindow][add]")
{
  editor::Main_Window w;
  editor::Gui_Controller c(&w);
  editor::connect_view_controller(w, c);

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  c.open(in.c_str());
  QTest::qWait(300);

  w.show();
  QTest::qWait(100);
  auto v = QApplication::focusWindow();
  REQUIRE(v);
  QTest::keyClick(v, Qt::Key_Right, Qt::NoModifier, 10);
  QTest::keyClick(v, Qt::Key_Down,  Qt::NoModifier, 10);
  QTest::keyClick(v, Qt::Key_Up,    Qt::NoModifier, 10);

  QTimer::singleShot(300, [&v]{
      auto v = QApplication::focusWindow();
      REQUIRE(v);
      QTest::keyClick(v, Qt::Key_A, Qt::AltModifier, 10);
      });

  QTimer::singleShot(600, [&w]{
      auto v = QApplication::modalWindow();
      REQUIRE(v);
      auto w = QApplication::focusWidget();
      REQUIRE(w);
      CHECK(v->title().toStdString() == "Add child");
      QTest::keyClicks(w, "blah", Qt::NoModifier, 10);
      QTest::keyClick(v, Qt::Key_Tab, Qt::NoModifier, 10);
      w = QApplication::focusWidget();
      REQUIRE(w);
      QTest::keyClicks(w, "blub", Qt::NoModifier, 10);
      QTest::keyClick(v, Qt::Key_Return, Qt::NoModifier, 10);
      });

  CHECK(w.isWindowModified() == false);

  QTest::keyClick(v, Qt::Key_E, Qt::AltModifier, 10);
  QTest::qWait(1000);

  CHECK(w.isWindowModified() == true);
  c.undo();
  CHECK(w.isWindowModified() == false);

}

TEST_CASE("mw tree view edit child undo clean", "[editor][qt][gui][mainwindow][add]")
{
  editor::Main_Window w;
  editor::Gui_Controller c(&w);
  editor::connect_view_controller(w, c);

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  c.open(in.c_str());
  QTest::qWait(300);

  w.show();
  QTest::qWait(100);
  auto v = QApplication::focusWindow();
  REQUIRE(v);
  QTest::keyClick(v, Qt::Key_Right, Qt::NoModifier, 10);
  QTest::keyClick(v, Qt::Key_Down,  Qt::NoModifier, 10);
  QTest::keyClick(v, Qt::Key_Right, Qt::NoModifier, 10);
  QTest::keyClick(v, Qt::Key_Down,  Qt::NoModifier, 10);

  QTimer::singleShot(300, [&v]{
      auto v = QApplication::focusWindow();
      REQUIRE(v);
      QTest::keyClick(v, Qt::Key_I, Qt::AltModifier, 10);
      });

  QTimer::singleShot(600, [&w]{
      auto v = QApplication::modalWindow();
      REQUIRE(v);
      auto w = QApplication::focusWidget();
      REQUIRE(w);
      CHECK(v->title().toStdString() == "Edit node");
      QTest::keyClick(v, Qt::Key_Home, Qt::ShiftModifier, 10);
      QTest::keyClicks(w, "blah", Qt::NoModifier, 10);
      QTest::keyClick(v, Qt::Key_Tab, Qt::NoModifier, 10);
      w = QApplication::focusWidget();
      REQUIRE(w);
      QTest::keyClicks(w, "blub", Qt::NoModifier, 10);
      QTest::keyClick(v, Qt::Key_Return, Qt::NoModifier, 10);
      });

  CHECK(w.isWindowModified() == false);

  QTest::keyClick(v, Qt::Key_E, Qt::AltModifier, 10);
  QTest::qWait(1000);

  CHECK(w.isWindowModified() == true);
  c.undo();
  CHECK(w.isWindowModified() == false);

}

TEST_CASE("mw toolbar on off", "[editor][gui][mainwindow][toolbar]")
{
  QSettings s;
  s.setValue("main_window/geometry", QByteArray());
  s.setValue("main_window/window_state", QByteArray());
  editor::Main_Window w;
  w.show();

  QTest::qWait(300);

  auto toolbar = w.findChild<QToolBar*>();
  REQUIRE(toolbar);
  CHECK(toolbar->isVisible() == true);


  auto show_tool_bar_action = w.findChild<QAction*>("show_tool_bar_action");
  REQUIRE(show_tool_bar_action);

  CHECK(show_tool_bar_action->isChecked() == true);

  show_tool_bar_action->setChecked(false);

  QTest::qWait(300);

  CHECK(toolbar->isVisible() == false);

  s.setValue("main_window/geometry", QByteArray());
  s.setValue("main_window/window_state", QByteArray());
}

TEST_CASE("mw toolbar menu update", "[editor][gui][mainwindow][toolbar]")
{
  QSettings s;
  s.setValue("main_window/geometry", QByteArray());
  s.setValue("main_window/window_state", QByteArray());
  editor::Main_Window w;
  w.show();

  QTest::qWait(300);

  auto toolbar = w.findChild<QToolBar*>();
  REQUIRE(toolbar);

  auto show_tool_bar_action = w.findChild<QAction*>("show_tool_bar_action");
  REQUIRE(show_tool_bar_action);

  CHECK(toolbar->isVisible() == true);
  CHECK(show_tool_bar_action->isChecked() == true);

  toolbar->setVisible(false);

  QTest::qWait(300);

  CHECK(show_tool_bar_action->isChecked() == false);

  s.setValue("main_window/geometry", QByteArray());
  s.setValue("main_window/window_state", QByteArray());
}

TEST_CASE("mw state", "[editor][gui][mainwindow][state]")
{
  QSettings s;
  s.setValue("main_window/geometry", QByteArray());
  s.setValue("main_window/window_state", QByteArray());

  QRect old_geometry;
  QRect new_geometry;
  {
    editor::Main_Window w;
    w.show();

    QTest::qWait(300);

    auto toolbar = w.findChild<QToolBar*>();
    REQUIRE(toolbar);
    toolbar->setVisible(false);

    old_geometry = w.geometry();
    new_geometry = old_geometry;
    new_geometry.setHeight(old_geometry.height()+10);
    w.setGeometry(new_geometry);

    QTest::qWait(300);
    w.close();
  }
  QTest::qWait(300);

  auto a = s.value("main_window/geometry");
  CHECK(a.isValid());
  CHECK(a.toByteArray().size() > 1);
  auto b = s.value("main_window/window_state");
  CHECK(b.isValid());
  CHECK(b.toByteArray().size() > 1);

  {
    editor::Main_Window w;
    w.show();

    QTest::qWait(300);

    auto g = w.geometry();
    CHECK(new_geometry.height() == g.height());
  }

  s.setValue("main_window/geometry", QByteArray());
  s.setValue("main_window/window_state", QByteArray());
}

