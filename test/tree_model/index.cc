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

#include <tree_model/index.hh>

#include <string>

using namespace std;

TEST_CASE("verify invalid properties", "[index][tree-model]" )
{
  tree_model::Index i;
  tree_model::Index j;
  CHECK(i == j);
  CHECK(i == i.first_child());
  CHECK(i == i.last_child());
  CHECK(i.column() == 0);
  CHECK(i.flags() == 0);
  CHECK(i.internal_id() == 0);
  CHECK(i.internal_pointer() == nullptr);
  CHECK(i.data().isValid() == false);
  CHECK(i.model() == nullptr);
  CHECK(i == i.parent());
  CHECK(i == i.prev_sibling());
  CHECK(i == i.next_sibling());
  CHECK(i == i.attribute(1));
}
