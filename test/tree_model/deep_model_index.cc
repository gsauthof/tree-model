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

#include <tree_model/deep_model_index.hh>
#include <tree_model/item_adaptor.hh>
#include <tree_model/xml.hh>

using namespace std;

TEST_CASE("Deep in out", "[deep][adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  auto root = a.index(0, 0);
  auto bar = root.child(1, 0);

  tree_model::Deep_Model_Index deep_bar(bar);
  CHECK(deep_bar.is_valid() == true);

  CHECK(QModelIndex(deep_bar) == bar);

}

TEST_CASE("Deep not valid", "[deep][tree-model]" )
{
  tree_model::Deep_Model_Index d;
  CHECK(d.is_valid() == false);
  tree_model::Deep_Model_Index e{QModelIndex()};
  CHECK(e.is_valid() == false);
  CHECK(QModelIndex(d) == QModelIndex(e));
}

TEST_CASE("Deep in out column", "[deep][adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  auto root = a.index(0, 0);
  auto bar = root.child(1, 1);

  tree_model::Deep_Model_Index deep_bar(bar);
  CHECK(deep_bar.is_valid() == true);

  CHECK(QModelIndex(deep_bar) == bar);

}
