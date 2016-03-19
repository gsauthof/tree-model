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

#include <string>

#include <QSignalSpy>
#include <QMenuBar>
#include <QSettings>

#include <editor/recent_menu.hh>

using namespace std;

TEST_CASE("recent basic", "[editor][qt][recent]")
{
  QSettings().remove("recent_files");

  editor::Recent_Menu m;
  m.set_size(3);

  CHECK(m.actions().empty() == true);

  m.register_file_open("foo.txt");
  m.register_file_open("/home/juser/bar.txt");

  CHECK(m.actions().size() == 2);

  m.register_file_open("x1.txt");
  CHECK(m.actions().size() == 3);
  m.register_file_open("x2.txt");
  REQUIRE(m.actions().size() == 3);

  auto l = m.actions();

  CHECK(l.at(0)->text().toUtf8().data() == string("&1. x2.txt"));
  CHECK(l.at(1)->text().toUtf8().data() == string("&2. x1.txt"));
  CHECK(l.at(2)->text().toUtf8().data() == string("&3. bar.txt"));
}

TEST_CASE("recent chosen", "[editor][qt][recent]")
{
  QSettings().remove("recent_files");

  editor::Recent_Menu m;
  m.set_size(3);

  CHECK(m.actions().empty() == true);

  m.register_file_open("foo.txt");
  m.register_file_open("bar.txt");
  m.register_file_open("/home/juser/x1.txt");
  m.register_file_open("x2.txt");
  REQUIRE(m.actions().size() == 3);

  QSignalSpy spy_triggered(&m, SIGNAL(chosen(QString)));

  auto l = m.actions();
  l.at(1)->trigger();
  REQUIRE(spy_triggered.size() == 1);
  CHECK(spy_triggered.front().front().toString().toUtf8().data()
      == string("/home/juser/x1.txt"));
}

TEST_CASE("recent chosen as submenu", "[editor][qt][recent]")
{
  QSettings().remove("recent_files");

  QMenuBar bar;
  QMenu *file_menu = bar.addMenu("&File");
  file_menu->addAction("&Open ...");
  QMenu *edit_menu = bar.addMenu("&Edit");
  edit_menu->addAction("&Preferences ...");
  
  editor::Recent_Menu *m = new editor::Recent_Menu(&bar);
  file_menu->addMenu(m);
  m->set_size(3);

  CHECK(m->actions().empty() == true);

  m->register_file_open("foo.txt");
  m->register_file_open("bar.txt");
  m->register_file_open("/home/juser/x1.txt");
  m->register_file_open("x2.txt");
  REQUIRE(m->actions().size() == 3);

  QSignalSpy spy_triggered(m, SIGNAL(chosen(QString)));

  auto l = m->actions();
  l.at(1)->trigger();
  REQUIRE(spy_triggered.size() == 1);
  CHECK(spy_triggered.front().front().toString().toUtf8().data()
      == string("/home/juser/x1.txt"));
}

TEST_CASE("recent persistence", "[editor][qt][recent]")
{
  QSettings().remove("recent_files");
  {
    QSettings s;
    s.beginWriteArray("recent_files");
    s.setArrayIndex(0);
    s.setValue("filename", "x2.txt");
    s.setArrayIndex(1);
    s.setValue("filename", "/home/juser/x1.txt");
    s.setArrayIndex(2);
    s.setValue("filename", "bar.txt");
    s.endArray();
  }

  editor::Recent_Menu m;
  m.set_size(3);

  // if this assertion fails then QSettings had trouble
  // to make the settings persistent - e.g. to write them into
  // the user's config directory, e.g. because the environment
  // variable HOME is not set
  REQUIRE(m.actions().size() == 3);

  auto l = m.actions();

  CHECK(l.at(0)->text().toUtf8().data() == string("&1. x2.txt"));
  CHECK(l.at(1)->text().toUtf8().data() == string("&2. x1.txt"));
  CHECK(l.at(2)->text().toUtf8().data() == string("&3. bar.txt"));

  CHECK(l.at(0)->data().toString().toUtf8().data() == string("x2.txt"));
  CHECK(l.at(1)->data().toString().toUtf8().data()
      == string("/home/juser/x1.txt"));
  CHECK(l.at(2)->data().toString().toUtf8().data() == string("bar.txt"));

}
