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

