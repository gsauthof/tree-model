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

#include <QStandardItemModel>
#include <QDebug>

#include <string>

using namespace std;

// in Qt, by convention, tree models have its hierachical
// information only in column 0
// (0, 1, p) index still has a parent and shares it with (0, 0, p),
// but (0, 1, p) does not have a child even if (0, 0, p) has one
TEST_CASE("qt tree not in col1", "[qtmodel]")
{
  QStandardItemModel model;
  //model.setColumnCount(2);

  // trying to archieve the same as with the standarditem interface
  /*
  QModelIndex parent;
  for (int i = 0; i < 2; ++i) {
    bool b = false;
    qDebug() << "parent: " << parent;
    qDebug() << "child: " << model.index(0, 0, parent);
    b = model.insertRow(0, parent);
    CHECK(b == true);
    qDebug() << "child after insert: " << model.index(0, 0, parent);
    b = model.setData(model.index(0, 0, parent), QVariant(QString("c0 %0").arg(i)));
    CHECK(b == true);
    b = model.setData(model.index(0, 1, parent), QVariant(QString("c1 %0").arg(i)));
    CHECK(b == true);
    parent = model.index(0, 0, parent);
  }
  */

  QStandardItem *root = model.invisibleRootItem();
  QStandardItem *parent = root;
  for (int i = 0; i < 4; ++i) {
    QStandardItem *itemc0 = new QStandardItem(QString("c0 %0").arg(i));
    QStandardItem *itemc1 = new QStandardItem(QString("c1 %0").arg(i));
    parent->appendRow(QList<QStandardItem*>() << itemc0 << itemc1);
    parent = itemc0;
  }

  auto x = model.index(0, 0, QModelIndex());
  CHECK(model.data(x).toString().toUtf8().data() == string("c0 0"));
  auto x1 = model.index(0, 1, QModelIndex());
  CHECK(model.data(x1).toString().toUtf8().data() == string("c1 0"));

  CHECK(model.index(0, 0, x).isValid() == true);
  CHECK(model.index(0, 0, x1).isValid() == false);
}

