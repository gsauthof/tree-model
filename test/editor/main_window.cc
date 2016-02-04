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

#include <editor/gui_controller.hh>
#include <editor/main_window.hh>
#include <editor/vc_matchmaker.hh>

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
