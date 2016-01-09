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

#include <tree_model/recorder.hh>
#include <tree_model/operation/set_data.hh>
#include <tree_model/operation/insert_rows.hh>
#include <tree_model/operation/remove_rows.hh>

#include <QSignalSpy>
#include <QDebug>

using namespace std;

TEST_CASE("set data operation", "[recorder][adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  auto root = a.index(0, 0);
  auto foo = root.child(0, 0);

  tree_model::operation::Set_Data sd(foo, foo, a);

  a.setData(foo, QVariant("fuu"));

  sd.update(foo, foo, a);

  CHECK(a.data(foo).toString().toUtf8().data() == string("fuu"));

  sd.rewind(a);
  CHECK(a.data(foo).toString().toUtf8().data() == string("foo"));

  sd.forward(a);
  CHECK(a.data(foo).toString().toUtf8().data() == string("fuu"));

  sd.rewind(a);
  CHECK(a.data(foo).toString().toUtf8().data() == string("foo"));

  sd.forward(a);
  CHECK(a.data(foo).toString().toUtf8().data() == string("fuu"));
}

TEST_CASE("set data operation content", "[recorder][adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  auto root = a.index(0, 0);
  auto foo = root.child(0, 1);

  tree_model::operation::Set_Data sd(foo, foo, a);

  a.setData(foo, QVariant("ehlo"));

  sd.update(foo, foo, a);

  CHECK(a.data(foo).toString().toUtf8().data() == string("ehlo"));

  sd.rewind(a);
  CHECK(a.data(foo).toString().toUtf8().data() == string("Hello"));

  sd.forward(a);
  CHECK(a.data(foo).toString().toUtf8().data() == string("ehlo"));

  sd.rewind(a);
  CHECK(a.data(foo).toString().toUtf8().data() == string("Hello"));

  sd.forward(a);
  CHECK(a.data(foo).toString().toUtf8().data() == string("ehlo"));
}

TEST_CASE("insert rows operation", "[recorder][adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  auto root = a.index(0, 0);

  CHECK(a.rowCount(root) == 2);
  a.insertRows(1, 3, root);
  CHECK(a.rowCount(root) == 5);
  CHECK(a.data(root.child(0, 0)).toString().toUtf8().data() == string("foo"));
  CHECK(a.data(root.child(4, 0)).toString().toUtf8().data() == string("bar"));
  tree_model::operation::Insert_Rows ir(root, 1, 3);

  ir.rewind(a);
  CHECK(a.rowCount(root) == 2);
  CHECK(a.data(root.child(0, 0)).toString().toUtf8().data() == string("foo"));
  CHECK(a.data(root.child(1, 0)).toString().toUtf8().data() == string("bar"));
  ir.forward(a);
  CHECK(a.rowCount(root) == 5);
}

TEST_CASE("remove rows operation", "[recorder][adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  auto root = a.index(0, 0);

  CHECK(a.rowCount(root) == 2);
  tree_model::operation::Remove_Rows rr(root, 0, 0, a);

  a.removeRows(0, 1, root);
  CHECK(a.rowCount(root) == 1);
  CHECK(a.data(root.child(0, 0)).toString().toUtf8().data() == string("bar"));

  rr.rewind(a);
  CHECK(a.rowCount(root) == 2);
  CHECK(a.data(root.child(0, 0)).toString().toUtf8().data() == string("foo"));
  CHECK(a.data(root.child(1, 0)).toString().toUtf8().data() == string("bar"));
  rr.forward(a);
  CHECK(a.rowCount(root) == 1);
}

TEST_CASE("remove rows operation last", "[recorder][adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  auto root = a.index(0, 0);

  CHECK(a.rowCount(root) == 2);
  tree_model::operation::Remove_Rows rr(root, 1, 1, a);

  a.removeRows(1, 1, root);
  CHECK(a.rowCount(root) == 1);
  CHECK(a.data(root.child(0, 0)).toString().toUtf8().data() == string("foo"));

  rr.rewind(a);
  CHECK(a.rowCount(root) == 2);
  CHECK(a.data(root.child(0, 0)).toString().toUtf8().data() == string("foo"));
  CHECK(a.data(root.child(1, 0)).toString().toUtf8().data() == string("bar"));
  rr.forward(a);
  CHECK(a.rowCount(root) == 1);
}

TEST_CASE("set data operation throws", "[recorder][adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  auto root = a.index(0, 0);
  auto foo = root.child(0, 0);

  tree_model::operation::Set_Data sd(foo, foo, a);

  a.setData(foo, QVariant("fuu"));

  sd.update(foo, foo, a);

  sd.rewind(a);
  CHECK_THROWS_AS(sd.rewind(a), std::logic_error);
}

TEST_CASE("set data operation throws fwd", "[recorder][adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  auto root = a.index(0, 0);
  auto foo = root.child(0, 0);

  tree_model::operation::Set_Data sd(foo, foo, a);

  a.setData(foo, QVariant("fuu"));

  sd.update(foo, foo, a);

  CHECK_THROWS_AS(sd.forward(a), std::logic_error);

  sd.rewind(a);
  sd.forward(a);
  CHECK_THROWS_AS(sd.forward(a), std::logic_error);
}

TEST_CASE("record set data", "[recorder][adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  tree_model::Recorder recorder;
  recorder.set_model(&a);

  auto root = a.index(0, 0);
  auto foo = root.child(0, 0);

  a.setData(foo, QVariant("fuu"));
  CHECK(a.data(foo).toString().toUtf8().data() == string("fuu"));
  // implicitly closes transaction of 2 edits
  recorder.rewind();
  CHECK(a.data(foo).toString().toUtf8().data() == string("foo"));
  recorder.forward();
  CHECK(a.data(foo).toString().toUtf8().data() == string("fuu"));
  recorder.rewind();
  CHECK(a.data(foo).toString().toUtf8().data() == string("foo"));
  recorder.forward();
  CHECK(a.data(foo).toString().toUtf8().data() == string("fuu"));
}

TEST_CASE("record auto commit", "[recorder][adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  tree_model::Recorder recorder;
  recorder.set_model(&a);

  auto root = a.index(0, 0);
  auto foo = root.child(0, 0);

  // auto-commit mode by default -> 2 transactions
  a.setData(foo, QVariant("fuu"));
  CHECK(a.data(foo).toString().toUtf8().data() == string("fuu"));
  a.setData(foo, QVariant("bar"));
  CHECK(a.data(foo).toString().toUtf8().data() == string("bar"));
  // implicitly closes transaction of 2 edits
  recorder.rewind();
  CHECK(a.data(foo).toString().toUtf8().data() == string("fuu"));
  recorder.forward();
  CHECK(a.data(foo).toString().toUtf8().data() == string("bar"));
  recorder.rewind();
  CHECK(a.data(foo).toString().toUtf8().data() == string("fuu"));
  recorder.rewind();
  CHECK(a.data(foo).toString().toUtf8().data() == string("foo"));
  CHECK_NOTHROW(recorder.rewind());
  CHECK(a.data(foo).toString().toUtf8().data() == string("foo"));
  recorder.forward();
  recorder.forward();
  CHECK(a.data(foo).toString().toUtf8().data() == string("bar"));
}

TEST_CASE("record transaction", "[recorder][adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  tree_model::Recorder recorder;
  QSignalSpy spy_begun(&recorder,
      SIGNAL(transaction_begun(const QString&)));
  QSignalSpy spy_committed(&recorder, SIGNAL(transaction_committed()));
  recorder.set_model(&a);

  auto root = a.index(0, 0);
  auto foo = root.child(1, 1);

  recorder.begin_transaction("one 4 all");
  REQUIRE(spy_begun.size() == 1);
  CHECK(spy_begun.back().front().toString().toUtf8().data()
      == string("one 4 all"));
  a.setData(foo, QVariant("hello"));
  CHECK(a.data(foo).toString().toUtf8().data() == string("hello"));
  a.setData(foo, QVariant("blah"));
  CHECK(a.data(foo).toString().toUtf8().data() == string("blah"));
  REQUIRE(spy_committed.size() == 0);
  recorder.commit();
  REQUIRE(spy_committed.size() == 1);
  recorder.rewind();
  CHECK(a.data(foo).toString().toUtf8().data() == string("World"));
  recorder.forward();
  CHECK(a.data(foo).toString().toUtf8().data() == string("blah"));
  recorder.rewind();
  CHECK(a.data(foo).toString().toUtf8().data() == string("World"));
  CHECK_NOTHROW(recorder.rewind());
  CHECK(a.data(foo).toString().toUtf8().data() == string("World"));
  recorder.forward();
  CHECK(a.data(foo).toString().toUtf8().data() == string("blah"));
}

TEST_CASE("record explicit transaction", "[recorder][adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  tree_model::Recorder recorder;
  QSignalSpy spy_rewind(&recorder, SIGNAL(rewind_enabled(bool)));
  QSignalSpy spy_forward(&recorder, SIGNAL(forward_enabled(bool)));
  QSignalSpy spy_model(&recorder, SIGNAL(model_changed(bool)));
  recorder.set_model(&a);
  REQUIRE(spy_rewind.size() == 1);
  CHECK(spy_rewind.back().back() == false);
  REQUIRE(spy_forward.size() == 1);
  CHECK(spy_forward.back().back() == false);
  REQUIRE(spy_model.size() == 1);
  CHECK(spy_model.back().back() == false);

  auto root = a.index(0, 0);
  auto foo = root.child(0, 0);

  recorder.begin_transaction("fuu edit");
  a.setData(foo, QVariant("fuu"));
  CHECK(a.data(foo).toString().toUtf8().data() == string("fuu"));

  REQUIRE(spy_rewind.size() == 2);
  CHECK(spy_rewind.back().back() == true);
  REQUIRE(spy_forward.size() == 1);
  CHECK(spy_forward.back().back() == false);
  REQUIRE(spy_model.size() == 2);
  CHECK(spy_model.back().back() == true);

  recorder.commit();
  recorder.begin_transaction("bar edit");
  a.setData(foo, QVariant("bar"));
  CHECK(a.data(foo).toString().toUtf8().data() == string("bar"));
  recorder.commit();

  REQUIRE(spy_rewind.size() == 2);
  CHECK(spy_rewind.back().back() == true);
  REQUIRE(spy_forward.size() == 1);
  CHECK(spy_forward.back().back() == false);

  recorder.rewind();
  CHECK(a.data(foo).toString().toUtf8().data() == string("fuu"));

  REQUIRE(spy_rewind.size() == 2);
  CHECK(spy_rewind.back().back() == true);
  REQUIRE(spy_forward.size() == 2);
  CHECK(spy_forward.back().back() == true);

  recorder.rewind();
  CHECK(a.data(foo).toString().toUtf8().data() == string("foo"));

  REQUIRE(spy_rewind.size() == 3);
  CHECK(spy_rewind.back().back() == false);
  REQUIRE(spy_forward.size() == 2);
  CHECK(spy_forward.back().back() == true);
  REQUIRE(spy_model.size() == 3);
  CHECK(spy_model.back().back() == false);

  recorder.forward();
  CHECK(a.data(foo).toString().toUtf8().data() == string("fuu"));

  REQUIRE(spy_rewind.size() == 4);
  CHECK(spy_rewind.back().back() == true);
  REQUIRE(spy_forward.size() == 2);
  CHECK(spy_forward.back().back() == true);

  recorder.forward();
  CHECK(a.data(foo).toString().toUtf8().data() == string("bar"));

  REQUIRE(spy_rewind.size() == 4);
  CHECK(spy_rewind.back().back() == true);
  REQUIRE(spy_forward.size() == 3);
  CHECK(spy_forward.back().back() == false);

  CHECK_NOTHROW(recorder.forward());
  CHECK(a.data(foo).toString().toUtf8().data() == string("bar"));

  REQUIRE(spy_rewind.size() == 4);
  CHECK(spy_rewind.back().back() == true);
  REQUIRE(spy_forward.size() == 3);
  CHECK(spy_forward.back().back() == false);
  REQUIRE(spy_model.size() == 4);
  CHECK(spy_model.back().back() == true);
}

TEST_CASE("rewind triggers data changed", "[recorder][adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);
  QSignalSpy spy_changed(&a, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)));

  tree_model::Recorder recorder;
  recorder.set_model(&a);

  auto root = a.index(0, 0);
  auto foo = root.child(0, 0);

  a.setData(foo, QVariant("fuu"));
  REQUIRE(spy_changed.size() == 1);
  CHECK(spy_changed.back().at(0) == foo);
  CHECK(spy_changed.back().at(1) == foo);
  CHECK(a.data(foo).toString().toUtf8().data() == string("fuu"));
  recorder.rewind();
  REQUIRE(spy_changed.size() == 2);
  CHECK(spy_changed.back().at(0) == foo);
  CHECK(spy_changed.back().at(1) == foo);
  CHECK(a.data(foo).toString().toUtf8().data() == string("foo"));
}

TEST_CASE("record insert rows", "[recorder][adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  tree_model::Recorder recorder;
  recorder.set_model(&a);

  auto root = a.index(0, 0);

  CHECK(a.rowCount(root) == 2);
  a.insertRows(2, 1, root);
  CHECK(a.rowCount(root) == 3);

  recorder.rewind();
  CHECK(a.rowCount(root) == 2);
  CHECK(a.data(root.child(0, 0)).toString().toUtf8().data() == string("foo"));
  CHECK(a.data(root.child(1, 0)).toString().toUtf8().data() == string("bar"));
  recorder.forward();
  CHECK(a.rowCount(root) == 3);
}

TEST_CASE("record remove rows", "[recorder][adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><a/><b/><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  tree_model::Recorder recorder;
  recorder.set_model(&a);

  auto root = a.index(0, 0);

  CHECK(a.rowCount(root) == 4);
  a.removeRows(1, 1, root);
  CHECK(a.rowCount(root) == 3);

  recorder.rewind();
  REQUIRE(a.rowCount(root) == 4);
  CHECK(a.data(root.child(0, 0)).toString().toUtf8().data() == string("foo"));
  CHECK(a.data(root.child(3, 0)).toString().toUtf8().data() == string("bar"));
  recorder.forward();
  CHECK(a.rowCount(root) == 3);
  CHECK(a.data(root.child(0, 0)).toString().toUtf8().data() == string("foo"));
  CHECK(a.data(root.child(2, 0)).toString().toUtf8().data() == string("bar"));
}

TEST_CASE("record remove rows transaction", "[recorder][adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><a/><b/><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  tree_model::Recorder recorder;
  recorder.set_model(&a);

  auto root = a.index(0, 0);

  CHECK(a.rowCount(root) == 4);
  recorder.begin_transaction("remove more rows");
  a.removeRows(1, 2, root);
  recorder.commit();
  CHECK(a.rowCount(root) == 2);

  recorder.rewind();
  REQUIRE(a.rowCount(root) == 4);
  CHECK(a.data(root.child(0, 0)).toString().toUtf8().data() == string("foo"));
  CHECK(a.data(root.child(3, 0)).toString().toUtf8().data() == string("bar"));
  recorder.forward();
  CHECK(a.rowCount(root) == 2);
  CHECK(a.data(root.child(0, 0)).toString().toUtf8().data() == string("foo"));
  CHECK(a.data(root.child(1, 0)).toString().toUtf8().data() == string("bar"));
}


TEST_CASE("record unlimit depth", "[recorder][adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><a/><b/><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  tree_model::Recorder recorder;
  recorder.set_model(&a);

  auto root = a.index(0, 0);
  auto foo = root.child(0, 1);

  recorder.begin_transaction("x1");
  a.setData(foo, QVariant("d1"));
  recorder.commit();
  recorder.begin_transaction("x2");
  a.setData(foo, QVariant("d2"));
  recorder.commit();
  recorder.begin_transaction("x3");
  a.setData(foo, QVariant("d3"));
  recorder.commit();

  CHECK(a.data(foo).toString().toUtf8().data() == string("d3"));
  recorder.rewind();
  CHECK(a.data(foo).toString().toUtf8().data() == string("d2"));
  recorder.rewind();
  CHECK(a.data(foo).toString().toUtf8().data() == string("d1"));
  recorder.rewind();
  CHECK(a.data(foo).toString().toUtf8().data() == string("Hello"));
  recorder.rewind();
  CHECK(a.data(foo).toString().toUtf8().data() == string("Hello"));
  recorder.forward();
  CHECK(a.data(foo).toString().toUtf8().data() == string("d1"));
  recorder.forward();
  CHECK(a.data(foo).toString().toUtf8().data() == string("d2"));
  recorder.forward();
  CHECK(a.data(foo).toString().toUtf8().data() == string("d3"));
  recorder.forward();
  CHECK(a.data(foo).toString().toUtf8().data() == string("d3"));
}

TEST_CASE("record limit depth", "[recorder][adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><a/><b/><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  tree_model::Recorder recorder;
  recorder.set_model(&a);
  recorder.set_depth(2);

  auto root = a.index(0, 0);
  auto foo = root.child(0, 1);

  recorder.begin_transaction("x1");
  a.setData(foo, QVariant("d1"));
  recorder.commit();
  recorder.begin_transaction("x2");
  a.setData(foo, QVariant("d2"));
  recorder.commit();
  recorder.begin_transaction("x3");
  a.setData(foo, QVariant("d3"));
  recorder.commit();

  QSignalSpy spy_rewind(&recorder, SIGNAL(rewind_enabled(bool)));

  CHECK(a.data(foo).toString().toUtf8().data() == string("d3"));
  recorder.rewind();
  CHECK(a.data(foo).toString().toUtf8().data() == string("d2"));
  CHECK(spy_rewind.empty() == true);
  recorder.rewind();
  CHECK(a.data(foo).toString().toUtf8().data() == string("d1"));
  REQUIRE(spy_rewind.empty() == false);
  CHECK(spy_rewind.front().front() == false);
  recorder.rewind();
  CHECK(a.data(foo).toString().toUtf8().data() == string("d1"));
  recorder.rewind();
  CHECK(a.data(foo).toString().toUtf8().data() == string("d1"));
  recorder.forward();
  REQUIRE(spy_rewind.size() == 2);
  CHECK(spy_rewind.back().front() == true);
  CHECK(a.data(foo).toString().toUtf8().data() == string("d2"));
  recorder.forward();
  CHECK(a.data(foo).toString().toUtf8().data() == string("d3"));
  recorder.forward();
  CHECK(a.data(foo).toString().toUtf8().data() == string("d3"));
}


