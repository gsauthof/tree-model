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
#include <QApplication>
#include <QMainWindow>
#include <QLineEdit>
#include <QTimer>
#include <QTime>

#include <boost/filesystem.hpp>

#include <editor/gui_command/save.hh>
#include <editor/gui_command/open.hh>
#include <tree_model/base.hh>


TEST_CASE("hello world", "[editor][qt][gui]")
{
  // Constructing a QApplication object on the stack for each
  // test case does not work - yields segfaults in
  // desktop geometry related code,
  // thus, QApplication should be constructed once
  // in a custom main function

  QLineEdit lineEdit;
  QTest::keyClicks(&lineEdit, "hello world");

  CHECK(lineEdit.text() == "hello world");
}

TEST_CASE("escape is ignored during save", "[editor][qt][gui][save]")
{
  std::string in(test::path::in() + "/small.xml");
  std::string out(test::path::out() + "/small.xml");
  boost::filesystem::create_directories(test::path::out());
  boost::filesystem::remove(out);
  boost::filesystem::copy(in, out);

  QString filename(out.c_str());

  QMainWindow w;
  w.show();
  editor::gui_command::Open o(&w);
  editor::gui_command::Save s(&w);

  QAbstractItemModel *model {nullptr};
  o.connect(&o, &editor::gui_command::Open::item_tree_model_created,
      [&model](QAbstractItemModel *m) { model = m; });

  o.connect(&o, &editor::gui_command::Open::tree_model_created,
      &s, &editor::gui_command::Save::set_tree_model);
  o.connect(&o, &editor::gui_command::Open::file_opened,
      &s, &editor::gui_command::Save::set_filename);
  o.connect(&o, &editor::gui_command::Open::file_type_opened,
      &s, &editor::gui_command::Save::set_file_type);
  o.open(filename);

  s.set_delay(3000);

  QTimer::singleShot(1000, [&w]{
      QTest::keyClick(w.focusWidget(), Qt::Key_Escape, Qt::NoModifier, 500);
      });
  QTime time;
  time.start();
  s.save();
  CHECK(time.elapsed() >= 3000);

  delete model;
}


