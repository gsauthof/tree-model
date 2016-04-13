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
#include <tree_model/protected_item_model.hh>

TEST_CASE("item read only adaptor", "[adaptor][xml][tree-model][protected]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Protected_Item_Model<tree_model::Item_Adaptor> a(m);

  CHECK(a.setData(a.index(0, 0).child(1, 0), QVariant("baz")) == true);
  CHECK(a.index(0, 0).child(1, 0).data().toString().toStdString() == "baz");
  auto is_edit = bool(a.index(0, 0).child(1, 0).flags() & Qt::ItemIsEditable);
  CHECK(is_edit == true);
  a.set_read_only(true);
  is_edit = bool(a.index(0, 0).child(1, 0).flags() & Qt::ItemIsEditable);
  CHECK(is_edit == false);
  CHECK(a.setData(a.index(0, 0).child(1, 0), QVariant("bat")) == false);
  CHECK(a.index(0, 0).child(1, 0).data().toString().toStdString() == "baz");
  a.set_read_only(false);
  CHECK(a.setData(a.index(0, 0).child(1, 0), QVariant("bat")) == true);
  CHECK(a.index(0, 0).child(1, 0).data().toString().toStdString() == "bat");
}

