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

#include <editor/command/open.hh>

#include <tree_model/item_adaptor.hh>
#include <tree_model/base.hh>

using namespace std;

TEST_CASE("open simple", "[editor][qt][command][open]")
{
  editor::command::Open o;

  QSignalSpy spy_file_opened(&o, SIGNAL(file_opened(const QString&)));
  qRegisterMetaType<QAbstractItemModel*>();
  QSignalSpy spy_item_tree_model_created(&o,
      SIGNAL(item_tree_model_created(QAbstractItemModel*)));
  qRegisterMetaType<tree_model::Base*>();
  QSignalSpy spy_tree_model_created(&o,
      SIGNAL(tree_model_created(tree_model::Base*)));

  std::string input(test::path::in() + "/small.xml");
  o.open(input.c_str());
  REQUIRE(spy_file_opened.size() == 1);
  CHECK(spy_file_opened.front().front().toString().toUtf8().data() == input);

  REQUIRE(spy_tree_model_created.size() == 1);
  REQUIRE(spy_item_tree_model_created.size() == 1);
  REQUIRE(qvariant_cast<QAbstractItemModel*>(
      spy_item_tree_model_created.front().at(0)) != nullptr);

  CHECK(
      qvariant_cast<QObject*>(
          spy_tree_model_created.front().at(0))->parent() ==
      qvariant_cast<QAbstractItemModel*>(
          spy_item_tree_model_created.front().at(0))
      );


  delete qvariant_cast<QAbstractItemModel*>(
      spy_item_tree_model_created.front().at(0));
}
