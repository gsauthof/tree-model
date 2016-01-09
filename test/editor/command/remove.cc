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

#include <editor/command/remove.hh>

#include <tree_model/item_adaptor.hh>
#include <tree_model/xml.hh>

using namespace std;

TEST_CASE("remove multiple", "[editor][qt][command][remove]")
{
  editor::command::Remove r;
  
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root>"
      "<foo>Hello</foo>"
      "<x>23</x>"
      "<bar>World</bar>"
      "<y>42</y>"
      "</root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  r.set_model(&a);


  QModelIndex root;
  auto doc_root = a.index(0, 0, root);

  CHECK(a.rowCount(doc_root) == 4);

  QModelIndexList l;
  l << doc_root.child(0, 0) << doc_root.child(2, 0);
  r.remove(l);

  REQUIRE(a.rowCount(doc_root) == 2);
  CHECK(a.data(doc_root.child(0, 1)).toString().toUtf8().data() == string("23"));
  CHECK(a.data(doc_root.child(1, 1)).toString().toUtf8().data() == string("42"));
}


TEST_CASE("remove one row multiple col", "[editor][qt][command][remove]")
{
  editor::command::Remove r;
  
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root>"
      "<foo>Hello</foo>"
      "<x>23</x>"
      "<bar>World</bar>"
      "<y>42</y>"
      "</root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  r.set_model(&a);


  QModelIndex root;
  auto doc_root = a.index(0, 0, root);

  CHECK(a.rowCount(doc_root) == 4);

  QModelIndexList l;
  l << doc_root.child(1, 0) << doc_root.child(1, 1);
  r.remove(l);

  REQUIRE(a.rowCount(doc_root) == 3);
  CHECK(a.data(doc_root.child(0, 1)).toString().toUtf8().data() == string("Hello"));
  CHECK(a.data(doc_root.child(1, 1)).toString().toUtf8().data() == string("World"));
}
