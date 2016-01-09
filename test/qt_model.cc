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

#include <QStringListModel>
#include <QSignalSpy>

#include <string>

using namespace std;

TEST_CASE("qt parent child back", "[qtmodel]" )
{
  QStringListModel model;
  QStringList list;
  list << "a" << "b" << "c";
  model.setStringList(list);

  auto doc_root = model.index(0, 0, QModelIndex());
  // is private:
  //REQUIRE(model.hasChildren(doc_root) == true);
  auto root = doc_root.parent();
  auto child = root.child(0, 0);
  REQUIRE(child != doc_root);
  REQUIRE(child == root);
  REQUIRE(child.isValid() == false);
  // is private:
  //REQUIRE(model.hasChildren(child) == true);
}

TEST_CASE("qt remove invalid", "[qtmodel]")
{
  QStringListModel model;
  QStringList list;
  list << "a" << "b" << "c";
  model.setStringList(list);

  QSignalSpy spy_begin(&model,
      SIGNAL(rowsAboutToBeRemoved(const QModelIndex&, int, int)));
  QSignalSpy spy_end(&model,
      SIGNAL(rowsRemoved(const QModelIndex&, int, int)));
  
  QModelIndex root;
  CHECK(model.removeRow(100, root) == false);

  REQUIRE(spy_begin.size() == 0);
  REQUIRE(spy_end.size() == 0);
}

TEST_CASE("qt sibling", "[qtmodel]")
{
  QStringListModel model;
  QStringList list;
  list << "a" << "b" << "c";
  model.setStringList(list);

  auto child = model.index(1, 0);
  CHECK(model.data(child, Qt::DisplayRole).toString().toUtf8().data()
      == string("b"));
  CHECK(child == child.sibling(child.row(), 0));
  auto first = child.sibling(0, 0);
  CHECK(first == model.index(0, 0));
}
