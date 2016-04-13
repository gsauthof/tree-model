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
#include <tree_model/qpmi_vector_model.hh>


using namespace std;

TEST_CASE("qpmi vector model basic", "[qpmiv][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><mid>23</mid><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  vector<QPersistentModelIndex> v;
  v.reserve(2);
  v.push_back(a.index(0, 0).child(0, 0));
  v.push_back(a.index(0, 0).child(2, 0));

  tree_model::QPMI_Vector_Model q(std::move(v));

  CHECK(q.rowCount() == 2);
  CHECK(q.columnCount() == 2);

  CHECK(q.index(0, 0).data().toString().toStdString() == "foo");
  CHECK(q.index(1, 0).data().toString().toStdString() == "bar");

  CHECK(q.index(0, 1).data().toString().toStdString() == "Hello");
  CHECK(q.index(1, 1).data().toString().toStdString() == "World");

  CHECK(q.index(0, 0).data(Qt::UserRole) == a.index(0, 0).child(0, 0));
  CHECK(q.index(1, 0).data(Qt::UserRole) == a.index(0, 0).child(2, 0));
  CHECK(q.index(0, 1).data(Qt::UserRole) == a.index(0, 0).child(0, 1));
  CHECK(q.index(1, 1).data(Qt::UserRole) == a.index(0, 0).child(2, 1));
}
