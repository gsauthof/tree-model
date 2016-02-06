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

#include <tree_model/item_adaptor.hh>
#include <tree_model/xml.hh>
#include <tree_model/util.hh>

using namespace std;

TEST_CASE("util descendents", "[util][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);


  using namespace tree_model::util;
  CHECK(descendents_less_than(a, QModelIndex(), 1) == false);
  CHECK(descendents_less_than(a, QModelIndex(), 100) == true);
  CHECK(descendents_less_than(a, QModelIndex(), 4) == true);
  CHECK(descendents_less_than(a, QModelIndex(), 3) == false);
  CHECK(descendents_less_than(a, QModelIndex(), 2) == false);

  CHECK(descendents_less_than(a, a.index(0, 0), 1) == false);
  CHECK(descendents_less_than(a, a.index(0, 0), 100) == true);
  CHECK(descendents_less_than(a, a.index(0, 0), 4) == true);
  CHECK(descendents_less_than(a, a.index(0, 0), 3) == true);
  CHECK(descendents_less_than(a, a.index(0, 0), 2) == false);
}
