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

#include <tree_model/xml.hh>

#include <string>

#include <QSignalSpy>

using namespace std;

TEST_CASE("Simple XML is loaded", "[xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML m(std::move(doc));

  tree_model::Index root;
  REQUIRE(m.has_children(root) == true);
  auto doc_root = m.first_child(root);
  REQUIRE(doc_root.is_valid());
  REQUIRE(root != doc_root);
  QVariant doc_root_name(doc_root.data());
  CHECK(string(doc_root_name.toString().toUtf8().data()) == "root");
  auto foo = doc_root.first_child();
  REQUIRE(foo.is_valid());
  QVariant foo_name(foo.data());
  CHECK(string(foo_name.toString().toUtf8().data()) == "foo");
  auto bar = foo.next_sibling();
  REQUIRE(bar.is_valid());
  QVariant bar_name(bar.data());
  CHECK(string(bar_name.toString().toUtf8().data()) == "bar");
  auto bar_parent = bar.parent();
  REQUIRE(bar_parent.model() == doc_root.model());
  REQUIRE(bar_parent.internal_pointer() == doc_root.internal_pointer());
}

TEST_CASE("Parent of document root is invalid", "[xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML m(std::move(doc));

  tree_model::Index root;
  REQUIRE(m.has_children(root) == true);
  auto doc_root = m.first_child(root);
  REQUIRE(doc_root.is_valid());
  REQUIRE(doc_root.parent().is_valid() == false);
  REQUIRE(doc_root.parent().internal_pointer() == nullptr);
  REQUIRE(root == doc_root.parent());
}

TEST_CASE("retrieve content", "[xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML m(std::move(doc));

  REQUIRE(m.has_children() == true);
  auto doc_root = m.first_child();
  REQUIRE(doc_root.is_valid());
  auto bar = doc_root.last_child();
  REQUIRE(bar.is_valid());
  auto bar_content = bar.attribute(1);
  REQUIRE(bar_content.is_valid());
  CHECK(string(bar_content.data().toString().toUtf8().data()) == "World");
}

TEST_CASE("parent child back", "[xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML m(std::move(doc));

  REQUIRE(m.has_children() == true);
  auto doc_root = m.first_child();
  auto root = doc_root.parent();
  auto child = root.first_child();
  REQUIRE(child != doc_root);
  REQUIRE(child == root);
  REQUIRE(child.is_valid() == false);
}

TEST_CASE("invalid has no flags", "[xml][tree-model]")
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML m(std::move(doc));
  REQUIRE(m.flags(tree_model::Index()) == 0);
}

TEST_CASE("xml same parents", "[xml][tree-model]")
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML m(std::move(doc));
  REQUIRE(m.flags(tree_model::Index()) == 0);
  auto doc_root = m.first_child();
  auto foo = doc_root.first_child();
  auto bar = doc_root.last_child();
  REQUIRE(m.parent(foo) == m.parent(bar));
}

TEST_CASE("xml different column, same parents", "[xml][tree-model]")
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML m(std::move(doc));
  REQUIRE(m.flags(tree_model::Index()) == 0);
  auto doc_root = m.first_child();
  auto foo = doc_root.first_child();
  auto bar = doc_root.last_child().attribute(1);
  REQUIRE(m.parent(foo) == m.parent(bar));
}

TEST_CASE("xml is editable", "[xml][tree-model]")
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML m(std::move(doc));
  REQUIRE(m.flags(tree_model::Index()) == 0);
  auto doc_root = m.first_child();
  Qt::ItemFlags r = 0;
  r = m.flags(doc_root) & Qt::ItemIsEditable;
  CHECK(r == Qt::ItemIsEditable);
  r = m.flags(m.attribute(doc_root, 1)) & Qt::ItemIsEditable;
  CHECK(r != Qt::ItemIsEditable);
  auto foo = doc_root.first_child();
  r = m.flags(foo) & Qt::ItemIsEditable;
  CHECK(r == Qt::ItemIsEditable);
  r = m.flags(m.attribute(foo, 1)) & Qt::ItemIsEditable;
  CHECK(r == Qt::ItemIsEditable);
  r = m.flags(tree_model::Index()) & Qt::ItemIsEditable;
  CHECK(r != Qt::ItemIsEditable);
}

TEST_CASE("xml set data", "[xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML m(std::move(doc));
  QSignalSpy spy_change(&m, SIGNAL(data_changed(const tree_model::Index&,
          const QVector<int>&)));

  auto doc_root = m.first_child();
  auto bar = doc_root.last_child();
  CHECK(bar.data().toString().toUtf8().data() == string("bar"));
  REQUIRE(m.set_data(bar, QVariant("baz")) == true);
  CHECK(bar.data().toString().toUtf8().data() == string("baz"));
  REQUIRE(spy_change.size() == 1);
  CHECK(qvariant_cast<tree_model::Index>(spy_change.front().front()) == bar);

  auto bar_content = doc_root.last_child().attribute(1);
  CHECK(bar_content.data().toString().toUtf8().data() == string("World"));
  REQUIRE(m.set_data(bar_content, QVariant("blah")) == true);
  CHECK(bar_content.data().toString().toUtf8().data() == string("blah"));
  REQUIRE(spy_change.size() == 2);
  CHECK(qvariant_cast<tree_model::Index>(spy_change.back().front())
      == bar_content);
}

TEST_CASE("xml set data that should fail", "[xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML m(std::move(doc));
  QSignalSpy spy_change(&m, SIGNAL(data_changed(const tree_model::Index&,
          const QVector<int>&)));

  CHECK(m.set_data(tree_model::Index(), QVariant("blah")) == false);
  REQUIRE(spy_change.empty() == true);
}

TEST_CASE("xml about to set data", "[xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML m(std::move(doc));

  QVariant before, after;
  m.connect(&m, &tree_model::XML::data_about_to_be_changed,
      [&m, &before](const tree_model::Index &i, const QVector<int>&) {
        before = m.data(i);
      });
  m.connect(&m, &tree_model::XML::data_changed,
      [&m, &after](const tree_model::Index &i, const QVector<int>&) {
        after = m.data(i);
      });

  QSignalSpy spy_change(&m, SIGNAL(data_changed(const tree_model::Index&,
          const QVector<int>&)));

  auto doc_root = m.first_child();
  auto bar = doc_root.last_child();

  CHECK(bar.data().toString().toUtf8().data() == string("bar"));
  REQUIRE(m.set_data(bar, QVariant("baz")) == true);
  CHECK(bar.data().toString().toUtf8().data() == string("baz"));

  CHECK(before.toString().toUtf8().data() == string("bar"));
  CHECK(after.toString().toUtf8().data() == string("baz"));
}

TEST_CASE("xml remove", "[xml][tree-model]")
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML m(std::move(doc));
  auto doc_root = m.first_child();
  {
    auto foo = doc_root.first_child();
    QVariant foo_name(foo.data());
    CHECK(string(foo_name.toString().toUtf8().data()) == "foo");

    CHECK(m.remove(foo) == true);
  }
  {
    auto foo = doc_root.first_child();
    QVariant foo_name(foo.data());
    CHECK(string(foo_name.toString().toUtf8().data()) == "bar");

    CHECK(foo == doc_root.last_child());
  }
}

TEST_CASE("xml remove with signals", "[xml][tree-model]")
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML m(std::move(doc));

  qRegisterMetaType<tree_model::Index>();
  QSignalSpy spy_begin(&m,
      SIGNAL(index_about_to_be_removed(const tree_model::Index &)));
  QSignalSpy spy_end(&m,
      SIGNAL(index_removed(const tree_model::Index &)));

  auto doc_root = m.first_child();

  auto foo = doc_root.first_child();
  CHECK(m.remove(foo) == true);

  REQUIRE(spy_begin.count() == 1);
  CHECK(qvariant_cast<tree_model::Index>(spy_begin.front().front()) == foo);
  REQUIRE(spy_end.count() == 1);
  CHECK(qvariant_cast<tree_model::Index>(spy_end.front().front()) == foo);
  // XXX also remove sibling? column > 0?
}


TEST_CASE("xml set data with child noop", "[xml][tree-model]")
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML m(std::move(doc));
  QSignalSpy spy_change(&m, SIGNAL(data_changed(const tree_model::Index&,
          const QVector<int>&)));

  auto doc_root = m.first_child();
  REQUIRE(m.set_data(m.attribute(doc_root, 1), QVariant("baz")) == false);
  REQUIRE(spy_change.empty() == true);
  CHECK(m.data(m.attribute(doc_root, 1)) == QVariant());
}


TEST_CASE("xml insert into empty model", "[xml][tree-model]")
{
  xxxml::doc::Ptr doc = xxxml::new_doc();
  tree_model::XML m(std::move(doc));
  QSignalSpy spy_begin(&m,
      SIGNAL(index_about_to_be_inserted(const tree_model::Index &, int)));
  QSignalSpy spy_end(&m, SIGNAL(index_inserted(const tree_model::Index &)));

  CHECK(m.has_children() == false);
  auto new_node = m.insert(tree_model::Index(), 1);
  REQUIRE(m.has_children() == true);
  CHECK(new_node == m.first_child());

  REQUIRE(spy_begin.size() == 1);
  CHECK(qvariant_cast<tree_model::Index>(spy_begin.front().front()) == tree_model::Index());
  CHECK(qvariant_cast<int>(spy_begin.front().back()) == 1);
  REQUIRE(spy_end.size() == 1);
  CHECK(qvariant_cast<tree_model::Index>(spy_end.front().front()) == new_node);

  CHECK(m.data(m.first_child()).toString().toUtf8().data() == string("NEW NODE"));
}


TEST_CASE("xml insert in between", "[xml][tree-model]")
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML m(std::move(doc));
  QSignalSpy spy_begin(&m,
      SIGNAL(index_about_to_be_inserted(const tree_model::Index &, int)));
  QSignalSpy spy_end(&m, SIGNAL(index_inserted(const tree_model::Index &)));

  auto root = m.first_child();

  auto new_node = m.insert(root.last_child(), -2);

  CHECK(new_node == root.first_child().next_sibling());

  REQUIRE(spy_begin.size() == 1);
  CHECK(qvariant_cast<tree_model::Index>(spy_begin.front().front()) == root.last_child());
  CHECK(qvariant_cast<int>(spy_begin.front().back()) == -2);
  REQUIRE(spy_end.size() == 1);
  CHECK(qvariant_cast<tree_model::Index>(spy_end.front().front()) == new_node);

  CHECK(m.data(root.first_child().next_sibling()).toString().toUtf8().data() == string("NEW NODE"));
  CHECK(m.data(root.first_child()).toString().toUtf8().data() == string("foo"));
  CHECK(m.data(root.last_child()).toString().toUtf8().data() == string("bar"));
}


TEST_CASE("xml no insert in text", "[xml][tree-model]")
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML m(std::move(doc));
  QSignalSpy spy_begin(&m,
      SIGNAL(index_about_to_be_inserted(const tree_model::Index &, int)));
  QSignalSpy spy_end(&m, SIGNAL(index_inserted(const tree_model::Index &)));

  auto root = m.first_child();

  auto new_node = m.insert(m.attribute(root.last_child(), 1), -1);

  CHECK(new_node == tree_model::Index());

  REQUIRE(spy_begin.empty() == true);
  REQUIRE(spy_end.empty() == true);
}

TEST_CASE("xml no whitespace after remove", "[xml][tree-model]")
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML m(std::move(doc));
  QSignalSpy spy_begin(&m,
      SIGNAL(index_about_to_be_inserted(const tree_model::Index &, int)));
  QSignalSpy spy_end(&m, SIGNAL(index_inserted(const tree_model::Index &)));

  auto root = m.first_child();

  CHECK(m.remove(root.first_child()) == true);
  CHECK(m.remove(root.first_child()) == true);
  CHECK(m.has_children(root) == false);
  CHECK(m.data(root.attribute(1)).isValid() == false);
}

TEST_CASE("xml header", "[xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML m(std::move(doc));

  CHECK(m.header_data(0, Qt::Horizontal).toString().toUtf8().data() == string("Key"));
  CHECK(m.header_data(1, Qt::Horizontal).toString().toUtf8().data() == string("Value"));
}

TEST_CASE("xml children in empty doc", "[xml][tree-model]")
{
  tree_model::XML m(xxxml::new_doc());
  auto i = m.last_child(tree_model::Index());
  CHECK(i.is_valid() == false);
  auto j = m.first_child(tree_model::Index());
  CHECK(j.is_valid() == false);
}
