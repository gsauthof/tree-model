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

#include <editor/command/open_xml.hh>
#include <editor/gui_command/write_aci.hh>


#include <QtTest/QtTest>
#include <QSignalSpy>
#include <QAbstractItemModel>
#include <QMainWindow>
#include <QProgressDialog>
#include <QTimer>

using namespace std;

TEST_CASE("writeaci gui basic", "[editor][gui][write-aci]")
{
  QMainWindow w;
  w.show();

  string in {test::path::in() + "/tap_3_12_small.xml"};
  auto r = editor::command::open_xml(in.c_str());
  unique_ptr<QAbstractItemModel> m(r.first);

  editor::gui_command::Write_ACI waci(&w);
  QSignalSpy spy_cancel(&waci, SIGNAL(was_canceled()));


  waci.set_model(m.get());
  waci.set_tree_model(r.second);
  editor::File_Type ft(editor::File_Type::XML);
  deque<string> asn_filenames = { test::path::in()
      + "/../../libgrammar/test/in/asn1/tap_3_12_strip.asn1"  };
  ft.set_asn_filenames(std::move(asn_filenames));
  waci.set_file_type(ft);
  waci.set_epoche(2);
  waci.set_delay(3500);
  QTimer::singleShot(1500, [&w]{
      auto v = w.findChild<QProgressDialog*>();
      REQUIRE(v);
      v->setValue(5);
      });
  QTimer::singleShot(3000, [&w]{
      auto v = w.findChild<QProgressDialog*>();
      REQUIRE(v);
      v->cancel();
      //QTest::keyClick(v, Qt::Key_Escape, Qt::NoModifier, 10);
      });

  waci.write();

  CHECK(spy_cancel.size() == 1);

}
