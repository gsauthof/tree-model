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

#include <tree_model/qmi_iterator.hh>

#include <sstream>
#include <string>

#include <algorithm>

using namespace std;

TEST_CASE("qmi iterator basic", "[tree-model][qmi][iterator]" )
{
  QStringListModel model;
  QStringList list;
  list << "foo" << "bar" << "foobar";
  list.sort();
  model.setStringList(list);

  auto b = tree_model::QMI_Iterator(model.index(0, 0));
  auto e = tree_model::QMI_Iterator(model.index(0, 0),
      model.rowCount(QModelIndex()));
  CHECK(b != e);
  CHECK(b == b);
  CHECK(e == e);

  ostringstream o;
  for (auto i = b; i != e; ++i)
    o << (*i).toString().toStdString() << ' ';

  CHECK(o.str() == "bar foo foobar ");
}

TEST_CASE("qmi iterator equal", "[tree-model][qmi][iterator]")
{
  QStringListModel model;
  QStringList list;
  list << "a" << "b" << "c" << "d" << "e" << "f" << "g" << "h" << "i" << "j";
  list.sort();
  model.setStringList(list);

  auto d1 = tree_model::QMI_Iterator(model.index(0, 0), 3);
  auto d2 = tree_model::QMI_Iterator(model.index(3, 0));
  auto e = tree_model::QMI_Iterator(model.index(0, 0),
      model.rowCount(QModelIndex()));

  CHECK(d1 == d2);
  CHECK(d1 != e);
  CHECK((*d1).toString().toStdString() == "d");
}

TEST_CASE("qmi random access", "[tree-model][qmi][iterator]" )
{
  QStringListModel model;
  QStringList list;
  list << "a" << "b" << "c" << "d" << "e" << "f" << "g" << "h" << "i" << "j";
  list.sort();
  model.setStringList(list);

  auto b = tree_model::QMI_Iterator(model.index(0, 0));
  auto e = tree_model::QMI_Iterator(model.index(0, 0),
      model.rowCount(QModelIndex()));

  auto x = b + 3;
  CHECK((*x).toString().toStdString() == "d");
  x += 6;
  CHECK((*x).toString().toStdString() == "j");
  ++x;
  CHECK(x == e);
  --x;
  CHECK((*x).toString().toStdString() == "j");

  auto n = e-b;
  CHECK(n == 10);
}

TEST_CASE("qmi lower upper", "[tree-model][qmi][iterator]")
{
  QStringListModel model;
  QStringList list;
  list << "foo" << "foobar" << "foobuz" << "fubar" << "bar" << "bozooka"
    << "zeta";
  list.sort();
  model.setStringList(list);

  auto b = tree_model::QMI_Iterator(model.index(0, 0));
  auto e = tree_model::QMI_Iterator(model.index(0, 0),
      model.rowCount(QModelIndex()));


  QVariant lv("foo");
  auto l = lower_bound(b, e, lv);
  REQUIRE(l != e);
  QVariant uv("foo");
  auto u = upper_bound(b, e, uv);
  REQUIRE(u != e);
  REQUIRE(l < u);
  CHECK(u-l == 1);
  CHECK((*l).toString().toStdString() == "foo");
}

TEST_CASE("qmi lower upper non equal", "[tree-model][qmi][iterator]")
{
  QStringListModel model;
  QStringList list;
  list << "foo" << "foobar" << "foobuz" << "fubar" << "bar" << "bozooka"
    << "zeta";
  list.sort();
  model.setStringList(list);

  auto b = tree_model::QMI_Iterator(model.index(0, 0));
  auto e = tree_model::QMI_Iterator(model.index(0, 0),
      model.rowCount(QModelIndex()));


  QVariant lv("foob");
  auto l = lower_bound(b, e, lv);
  REQUIRE(l != e);
  QVariant uv("foob");
  auto u = upper_bound(b, e, uv);
  REQUIRE(u != e);
  CHECK(l == u);
  CHECK(u-l == 0);
  CHECK((*l).toString().toStdString() == "foobar");
}

TEST_CASE("qmi lower upper prefix", "[tree-model][qmi][iterator]")
{
  QStringListModel model;
  QStringList list;
  list << "foo" << "foobar" << "foobuz" << "fubar" << "bar" << "bozooka"
    << "zeta";
  list.sort();
  model.setStringList(list);

  auto b = tree_model::QMI_Iterator(model.index(0, 0));
  auto e = tree_model::QMI_Iterator(model.index(0, 0),
      model.rowCount(QModelIndex()));


  QString lv("foob");
  auto l = lower_bound(b, e, lv, [](const QVariant &x, const QString &v) {
      auto t = x.toString().left(v.size());
      return t < v;
      } );
  REQUIRE(l != e);
  auto u = upper_bound(b, e, lv, [](const QString &v, const QVariant &x) {
      auto t = x.toString().left(v.size());
      return v < t;
      } );
  REQUIRE(u != e);
  CHECK(l < u);
  CHECK(u-l == 2);
  CHECK((*l).toString().toStdString() == "foobar");
}

