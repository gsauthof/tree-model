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

#include <tree_model/df_qmi_traverser.hh>
#include <tree_model/item_adaptor.hh>
#include <tree_model/xml.hh>

#include <sstream>

using namespace std;

TEST_CASE("dfqmi basic", "[qmi][dfs][traverser][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><x><a>23</a><b><c>42</c></b></x><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  auto root = a.index(0, 0);

  tree_model::DF_QMI_Traverser df(root);

  ostringstream o;

  while (!df.eot()) {
    o << a.data(*df).toString().toStdString() << ' ';
    df.advance();
  }

  CHECK(o.str() == "root foo x a b c bar ");

}
