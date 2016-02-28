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

#include <QSignalSpy>
#include <QMimeData>
#include <string>


using namespace std;

TEST_CASE("Simple adaptor usage", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);
  QModelIndex root;
  REQUIRE(a.hasChildren(root) == true);
  REQUIRE(a.rowCount(root) == 1);
  REQUIRE(a.columnCount(root) == 2);
  auto doc_root = a.index(0, 0, root);
  REQUIRE(doc_root.isValid());
  REQUIRE(doc_root.data().toString().toUtf8().data() == string("root"));
  REQUIRE(a.rowCount(doc_root) == 2);
  auto bar = a.index(1, 0, doc_root);
  REQUIRE(bar.isValid());
  REQUIRE(bar.row() == 1);
  REQUIRE(bar.data().toString().toUtf8().data() == string("bar"));
  auto bar_content = a.index(1, 1, doc_root);
  REQUIRE(bar_content.row() == 1);
  REQUIRE(bar_content.column() == 1);
  REQUIRE(bar_content.data().toString().toUtf8().data() == string("World"));
}

TEST_CASE("adaptor parent child back", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  auto doc_root = a.index(0, 0, QModelIndex());
  REQUIRE(a.hasChildren(doc_root) == true);
  auto root = doc_root.parent();
  auto child = root.child(0, 0);
  REQUIRE(child != doc_root);
  REQUIRE(child == root);
  REQUIRE(child.isValid() == false);
  REQUIRE(a.hasChildren(child) == true);
}

TEST_CASE("model ownership", "[adaptor][tree-model]")
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  REQUIRE(static_cast<QObject*>(m)->parent() == nullptr);
  tree_model::Item_Adaptor a(m);
  REQUIRE(static_cast<QObject*>(m)->parent() != nullptr);
}

TEST_CASE("create qm index", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  auto doc_root = a.index(0, 0, QModelIndex());
  auto child = doc_root.child(0, 0);
  auto parent = a.parent(child);
  REQUIRE(doc_root == parent);
}

TEST_CASE("same parents", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  auto doc_root = a.index(0, 0, QModelIndex());
  auto foo = doc_root.child(0, 0);
  auto bar = doc_root.child(1, 0);
  REQUIRE(a.parent(foo) == a.parent(bar));
}

TEST_CASE("same model", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  auto doc_root = a.index(0, 0, QModelIndex());
  auto foo = doc_root.child(0, 0);
  auto bar = doc_root.child(1, 0);
  REQUIRE(foo.model() == bar.model());
}

TEST_CASE("different column, same parents", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  auto doc_root = a.index(0, 0, QModelIndex());
  auto foo = doc_root.child(0, 0);
  auto bar = doc_root.child(0, 1);
  REQUIRE(a.parent(foo) == a.parent(bar));
}

TEST_CASE("rank is in tooltip role", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  auto doc_root = a.index(0, 0, QModelIndex());
  auto bar = doc_root.child(1, 0);
  REQUIRE(a.data(bar, Qt::ToolTipRole).toString().toUtf8().data() == string("#2"));
}

TEST_CASE("is editable", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  auto doc_root = a.index(0, 0, QModelIndex());
  Qt::ItemFlags r = a.flags(doc_root) & Qt::ItemIsEditable;
  CHECK(r == Qt::ItemIsEditable);
  auto bar = doc_root.child(1, 0);
  r = a.flags(bar) & Qt::ItemIsEditable;
  CHECK(r == Qt::ItemIsEditable);
  r = a.flags(doc_root.child(1, 1)) & Qt::ItemIsEditable;
  CHECK(r == Qt::ItemIsEditable);
  r = a.flags(a.index(0, 1, QModelIndex())) & Qt::ItemIsEditable;
  CHECK(r != Qt::ItemIsEditable);
  r = a.flags(QModelIndex()) & Qt::ItemIsEditable;
  CHECK(r != Qt::ItemIsEditable);
}

TEST_CASE("set data", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);
  QSignalSpy spy_change(&a, SIGNAL(dataChanged(const QModelIndex&,
          const QModelIndex&, const QVector<int> &)));

  auto doc_root = a.index(0, 0, QModelIndex());
  auto bar = doc_root.child(1, 0);
  CHECK(bar.data().toString().toUtf8().data() == string("bar"));
  REQUIRE(a.setData(bar, QVariant("baz")) == true);
  CHECK(bar.data().toString().toUtf8().data() == string("baz"));
  REQUIRE(spy_change.size() == 1);
  CHECK(spy_change.back().at(0) == spy_change.back().at(1));
  CHECK(qvariant_cast<QModelIndex>(spy_change.back().front()) == bar);

  auto bar_content = doc_root.child(1, 1);
  CHECK(bar_content.data().toString().toUtf8().data() == string("World"));
  REQUIRE(a.setData(bar_content, QVariant("blah")) == true);
  CHECK(bar_content.data().toString().toUtf8().data() == string("blah"));
  REQUIRE(spy_change.size() == 2);
  CHECK(spy_change.back().at(0) == spy_change.back().at(1));
  CHECK(spy_change.back().front() == bar_content);
}

TEST_CASE("set data that should fail", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);
  QSignalSpy spy_change(&a, SIGNAL(dataChanged(const QModelIndex&,
          const QModelIndex&, const QVector<int> &)));

  CHECK(a.setData(QModelIndex(), QVariant("blah")) == false);
  CHECK(spy_change.empty() == true);
}

TEST_CASE("remove", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  QSignalSpy spy_begin(&a,
      SIGNAL(rowsAboutToBeRemoved(const QModelIndex&, int, int)));
  QSignalSpy spy_end(&a,
      SIGNAL(rowsRemoved(const QModelIndex&, int, int)));

  QModelIndex root;
  auto doc_root = a.index(0, 0, root);
  CHECK(a.rowCount(doc_root) == 2);
  auto foo = a.index(0, 0, doc_root);
  (void)foo;
  CHECK(a.removeRow(0, doc_root) == true);
  CHECK(a.rowCount(doc_root) == 1);
  auto bar = a.index(0, 0, doc_root);
  REQUIRE(bar.isValid());
  REQUIRE(bar.row() == 0);
  REQUIRE(bar.data().toString().toUtf8().data() == string("bar"));

  REQUIRE(spy_begin.size() == 1);
  CHECK(qvariant_cast<QModelIndex>(spy_begin.front().at(0)) == doc_root);
  CHECK(spy_begin.front().at(1).toInt() == 0);
  CHECK(spy_begin.front().at(2).toInt() == 0);
  REQUIRE(spy_end.size() == 1);
  CHECK(qvariant_cast<QModelIndex>(spy_end.front().at(0)) == doc_root);
  CHECK(spy_end.front().at(1).toInt() == 0);
  CHECK(spy_end.front().at(2).toInt() == 0);
}

TEST_CASE("remove rows", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  QSignalSpy spy_begin(&a,
      SIGNAL(rowsAboutToBeRemoved(const QModelIndex&, int, int)));
  QSignalSpy spy_end(&a,
      SIGNAL(rowsRemoved(const QModelIndex&, int, int)));

  QModelIndex root;
  auto doc_root = a.index(0, 0, root);
  CHECK(a.rowCount(doc_root) == 2);
  auto foo = a.index(0, 0, doc_root);
  (void)foo;
  CHECK(a.removeRows(0, 2, doc_root) == true);
  CHECK(a.rowCount(doc_root) == 0);

  // Currently implemented as simple loop in the adaptor
  // -> multiple signals are emitted
  REQUIRE(spy_begin.size() == 2);
  CHECK(qvariant_cast<QModelIndex>(spy_begin.front().at(0)) == doc_root);
  CHECK(spy_begin.front().at(1).toInt() == 0);
  CHECK(spy_begin.front().at(2).toInt() == 0);
  CHECK(spy_begin.back().at(1).toInt() == 0);
  CHECK(spy_begin.back().at(2).toInt() == 0);
  REQUIRE(spy_end.size() == 2);
  CHECK(qvariant_cast<QModelIndex>(spy_end.front().at(0)) == doc_root);
  CHECK(spy_end.front().at(1).toInt() == 0);
  CHECK(spy_end.front().at(2).toInt() == 0);
  CHECK(spy_end.back().at(1).toInt() == 0);
  CHECK(spy_end.back().at(2).toInt() == 0);
}

TEST_CASE("remove invalid", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  QSignalSpy spy_begin(&a,
      SIGNAL(rowsAboutToBeRemoved(const QModelIndex&, int, int)));
  QSignalSpy spy_end(&a,
      SIGNAL(rowsRemoved(const QModelIndex&, int, int)));

  QModelIndex root;
  auto doc_root = a.index(0, 0, root);
  CHECK(a.rowCount(doc_root) == 2);

  CHECK(a.removeRow(2, doc_root) == false);

  CHECK(a.rowCount(doc_root) == 2);

  REQUIRE(spy_begin.size() == 0);
  REQUIRE(spy_end.size() == 0);
}

TEST_CASE("row count in empty model", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::new_doc();
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);
  CHECK(a.rowCount(QModelIndex()) == 0);
}

TEST_CASE("insert row into empty model", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::new_doc();
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);
  QSignalSpy spy_begin(&a,
      SIGNAL(rowsAboutToBeInserted(const QModelIndex &, int, int)));
  QSignalSpy spy_end(&a, SIGNAL(rowsInserted(const QModelIndex &, int, int)));

  CHECK(a.insertRow(0) == true);

  REQUIRE(spy_begin.size() == 1);
  REQUIRE(spy_end.size() == 1);
  CHECK(qvariant_cast<QModelIndex>(spy_begin.front().front()) == QModelIndex());
  CHECK(qvariant_cast<int>(spy_begin.front().at(1)) == 0);
  CHECK(qvariant_cast<int>(spy_begin.front().at(2)) == 0);
  CHECK(qvariant_cast<QModelIndex>(spy_end.front().front()) == QModelIndex());
  CHECK(qvariant_cast<int>(spy_end.front().at(1)) == 0);
  CHECK(qvariant_cast<int>(spy_end.front().at(2)) == 0);

  CHECK(a.data(a.index(0, 0)).toString().toUtf8().data() == string("NEW NODE"));
}


TEST_CASE("insert in between", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);
  QSignalSpy spy_begin(&a,
      SIGNAL(rowsAboutToBeInserted(const QModelIndex &, int, int)));
  QSignalSpy spy_end(&a, SIGNAL(rowsInserted(const QModelIndex &, int, int)));

  CHECK(a.insertRow(1, a.index(0, 0)) == true);

  CHECK(a.rowCount(a.index(0, 0)) == 3);

  REQUIRE(spy_begin.size() == 1);
  REQUIRE(spy_end.size() == 1);
  CHECK(qvariant_cast<QModelIndex>(spy_begin.front().front()) == a.index(0, 0));
  CHECK(qvariant_cast<int>(spy_begin.front().at(1)) == 1);
  CHECK(qvariant_cast<int>(spy_begin.front().at(2)) == 1);
  CHECK(qvariant_cast<QModelIndex>(spy_end.front().front()) == a.index(0, 0));
  CHECK(qvariant_cast<int>(spy_end.front().at(1)) == 1);
  CHECK(qvariant_cast<int>(spy_end.front().at(2)) == 1);

  CHECK(a.data(a.index(0, 0, a.index(0, 0))).toString().toUtf8().data() == string("foo"));
  CHECK(a.data(a.index(1, 0, a.index(0, 0))).toString().toUtf8().data() == string("NEW NODE"));

  CHECK(a.data(a.index(2, 0, a.index(0, 0))).toString().toUtf8().data() == string("bar"));
}

TEST_CASE("insert last", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);
  QSignalSpy spy_begin(&a,
      SIGNAL(rowsAboutToBeInserted(const QModelIndex &, int, int)));
  QSignalSpy spy_end(&a, SIGNAL(rowsInserted(const QModelIndex &, int, int)));

  CHECK(a.insertRow(2, a.index(0, 0)) == true);

  CHECK(a.rowCount(a.index(0, 0)) == 3);

  REQUIRE(spy_begin.size() == 1);
  REQUIRE(spy_end.size() == 1);
  CHECK(qvariant_cast<QModelIndex>(spy_begin.front().front()) == a.index(0, 0));
  CHECK(qvariant_cast<int>(spy_begin.front().at(1)) == 2);
  CHECK(qvariant_cast<int>(spy_begin.front().at(2)) == 2);
  CHECK(qvariant_cast<QModelIndex>(spy_end.front().front()) == a.index(0, 0));
  CHECK(qvariant_cast<int>(spy_end.front().at(1)) == 2);
  CHECK(qvariant_cast<int>(spy_end.front().at(2)) == 2);

  CHECK(a.data(a.index(0, 0, a.index(0, 0))).toString().toUtf8().data() == string("foo"));
  CHECK(a.data(a.index(2, 0, a.index(0, 0))).toString().toUtf8().data() == string("NEW NODE"));

  CHECK(a.data(a.index(1, 0, a.index(0, 0))).toString().toUtf8().data() == string("bar"));
}


TEST_CASE("persistent after insert", "[adaptor][xml][tree-model]")
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root>\n"
      "  <cat1>\n"
      "    <a>1</a>\n"
      "    <b>2</b>\n"
      "    <c>3</c>\n"
      "  </cat1>\n"
      "  <cat2>\n"
      "    <d>4</d>\n"
      "    <e>5</e>\n"
      "    <f>6</f>\n"
      "  </cat2>\n"
      "</root>\n");
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);
  QSignalSpy spy_begin(&a,
      SIGNAL(rowsAboutToBeInserted(const QModelIndex &, int, int)));
  QSignalSpy spy_end(&a, SIGNAL(rowsInserted(const QModelIndex &, int, int)));

  auto cat1 = a.index(0, 0, a.index(0, 0));
  QPersistentModelIndex p(cat1);

  CHECK(a.insertRow(a.rowCount(cat1), cat1) == true);

  QModelIndex new_cat1(p);

  CHECK(new_cat1.row() == 0);
  CHECK(new_cat1 == a.index(0, 0, a.index(0, 0)));
}

TEST_CASE("set data about to be", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);
  QVariant before, after;
  a.connect(&a, &tree_model::Item_Adaptor::dataAboutToBeChanged,
      [&a, &before](const QModelIndex &topLeft,
          const QModelIndex & bottomRight,
          const QVector<int> &roles){
        before = a.data(topLeft);
      });
  a.connect(&a, &tree_model::Item_Adaptor::dataChanged,
      [&a, &after](const QModelIndex &topLeft,
          const QModelIndex & bottomRight,
          const QVector<int> &roles){
        after = a.data(topLeft);
      });

  auto doc_root = a.index(0, 0, QModelIndex());
  auto bar = doc_root.child(1, 0);

  REQUIRE(a.setData(bar, QVariant("baz")) == true);
  CHECK(before.toString().toUtf8().data() == string("bar"));
  CHECK(after.toString().toUtf8().data() == string("baz"));
}

TEST_CASE("mime type", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  auto l = a.mimeTypes();
  REQUIRE(l.size() == 1);
  CHECK(l.front().toUtf8().data() == string("text/xml"));
}

TEST_CASE("mime data no indexes", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  QMimeData *r = a.mimeData(QModelIndexList());
  // as specified by the Qt API doc
  CHECK(r == nullptr);
}

TEST_CASE("mime data", "[adaptor][xml][tree-model]" )
{
  const char inp[] = "<root><foo>Hello</foo><bar>World</bar></root>";
  xxxml::doc::Ptr doc = xxxml::read_memory(inp);
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  QMimeData *r = a.mimeData(QModelIndexList() << QModelIndex());
  CHECK(r != nullptr);

  QByteArray d(r->data("text/xml"));
  QString s(d);
  s.remove(' ');
  s.remove('\n');
  CHECK(s.toUtf8().data() == "<?xmlversion=\"1.0\"?>" + string(inp));

  delete r;
}

TEST_CASE("mime data subtree", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar><a>Worl</a><b>d</b></bar></root>");
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  QMimeData *r = a.mimeData(QModelIndexList() << a.index(0,0).child(1,0));
  CHECK(r != nullptr);

  QByteArray d(r->data("text/xml"));
  QString s(d);
  s.remove(' ');
  s.remove('\n');
  CHECK(s.toUtf8().data() == string("<bar><a>Worl</a><b>d</b></bar>"));

  delete r;
}

TEST_CASE("mime drop", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo></root>");
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);
  QSignalSpy spy_insert(&a, SIGNAL(rowsAboutToBeInserted(const QModelIndex &,
          int, int)));
  QSignalSpy spy_inserted(&a, SIGNAL(rowsInserted(const QModelIndex &,
          int, int)));

  const char inp[] = "<bar>World</bar>";
  QByteArray b(inp, sizeof(inp)-1);
  QMimeData md;
  md.setData("text/xml", b);
  CHECK(a.rowCount(a.index(0, 0)) == 1);
  REQUIRE(a.dropMimeData(&md, Qt::CopyAction, -1, -1, a.index(0, 0)) == true);
  CHECK(a.rowCount(a.index(0, 0)) == 2);
  CHECK(a.data(a.index(0, 0).child(1, 1)).toString().toUtf8().data() == string("World"));

  REQUIRE(spy_insert.size() == 1);
  CHECK(spy_insert.back().at(0) == a.index(0, 0));
  CHECK(spy_insert.back().at(1) == 1);
  CHECK(spy_insert.back().at(2) == 1);
  REQUIRE(spy_inserted.size() == 1);
  CHECK(spy_inserted.back().at(0) == a.index(0, 0));
  CHECK(spy_inserted.back().at(1) == 1);
  CHECK(spy_inserted.back().at(2) == 1);
}

TEST_CASE("mime drop before", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo></root>");
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);
  QSignalSpy spy_insert(&a, SIGNAL(rowsAboutToBeInserted(const QModelIndex &,
          int, int)));
  QSignalSpy spy_inserted(&a, SIGNAL(rowsInserted(const QModelIndex &,
          int, int)));

  const char inp[] = "<bar>World</bar>";
  QByteArray b(inp, sizeof(inp)-1);
  QMimeData md;
  md.setData("text/xml", b);
  CHECK(a.rowCount(a.index(0, 0)) == 1);
  REQUIRE(a.dropMimeData(&md, Qt::CopyAction, 0, 0, a.index(0, 0)) == true);
  CHECK(a.rowCount(a.index(0, 0)) == 2);
  CHECK(a.data(a.index(0, 0).child(1, 1)).toString().toUtf8().data() == string("Hello"));

  REQUIRE(spy_insert.size() == 1);
  CHECK(spy_insert.back().at(0) == a.index(0, 0));
  CHECK(spy_insert.back().at(1) == 0);
  CHECK(spy_insert.back().at(2) == 0);
  REQUIRE(spy_inserted.size() == 1);
  CHECK(spy_inserted.back().at(0) == a.index(0, 0));
  CHECK(spy_inserted.back().at(1) == 0);
  CHECK(spy_inserted.back().at(2) == 0);
}

TEST_CASE("mime drop into empty doc", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::new_doc();
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);
  QSignalSpy spy_insert(&a, SIGNAL(rowsAboutToBeInserted(const QModelIndex &,
          int, int)));
  QSignalSpy spy_inserted(&a, SIGNAL(rowsInserted(const QModelIndex &,
          int, int)));

  const char inp[] = "<root><foo>Hello</foo><bar>World</bar></root>";
  QByteArray b(inp, sizeof(inp)-1);
  QMimeData md;
  md.setData("text/xml", b);
  CHECK(a.rowCount(QModelIndex()) == 0);
  REQUIRE(a.dropMimeData(&md, Qt::CopyAction, -1, -1, QModelIndex()) == true);
  REQUIRE(a.rowCount(a.index(0, 0)) == 2);
  CHECK(a.data(a.index(0, 0).child(1, 1)).toString().toUtf8().data() == string("World"));

  REQUIRE(spy_insert.size() == 1);
  CHECK(spy_insert.back().at(0) == QModelIndex());
  CHECK(spy_insert.back().at(1) == 0);
  CHECK(spy_insert.back().at(2) == 0);
  REQUIRE(spy_inserted.size() == 1);
  CHECK(spy_inserted.back().at(0) == QModelIndex());
  CHECK(spy_inserted.back().at(1) == 0);
  CHECK(spy_inserted.back().at(2) == 0);
}

TEST_CASE("adaptor header", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);
  CHECK(a.headerData(0, Qt::Horizontal).toString().toUtf8().data() == string("Key"));
  CHECK(a.headerData(1, Qt::Horizontal).toString().toUtf8().data() == string("Value"));

}

TEST_CASE("adaptor parent of root", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><x>...</x><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  QModelIndex root;
  CHECK(root.isValid() == false);

  QModelIndex p_root(root.parent());
  CHECK(root.isValid() == false);

  QModelIndex i(a.index(0, 0, p_root));
  CHECK(i.isValid() == true);
}

TEST_CASE("adaptor sibling", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><x>...</x><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  QModelIndex root;
  CHECK(root.isValid() == false);
  QModelIndex root_right(a.sibling(1, 0, root));
  CHECK(root_right.isValid() == false);
  QModelIndex root_left(a.sibling(0, 0, root));
  // this is true because:
  // a.sibling(0, 0, root) <=> a.index(0, 0, a.parent(root))
  CHECK(root_left.isValid() == true);
  QModelIndex root_same(a.sibling(-1, -1, root));
  CHECK(root_same.isValid() == false);
  CHECK(root_same == root);

  auto doc_root = a.index(0, 0, root);
  QModelIndex doc_root_right(a.sibling(1, 0, doc_root));
  CHECK(doc_root_right.isValid() == false);
  QModelIndex doc_root_same(a.sibling(0, 0, doc_root));
  CHECK(doc_root_same == doc_root);

  auto x = a.index(1, 0, doc_root);
  auto x_left = a.sibling(0, 0, x);
  CHECK(a.data(x_left).toString().toUtf8().data() == string("foo"));
  auto x_right = a.sibling(2, 0, x);
  CHECK(a.data(x_right).toString().toUtf8().data() == string("bar"));
  auto x_same = a.sibling(1, 0, x);
  CHECK(a.data(x_same).toString().toUtf8().data() == string("x"));
  CHECK(x == x_same);
  auto x_col = a.sibling(2, 1, x);
  CHECK(a.data(x_col).toString().toUtf8().data() == string("World"));
  auto x_invalid = a.sibling(3, 0, x);
  CHECK(x_invalid.isValid() == false);
  CHECK(x != x_invalid);
  auto x_invalid2 = a.sibling(-1, 0, x);
  CHECK(x_invalid2.isValid() == false);
  CHECK(x != x_invalid2);
  auto x_invalid_col = a.sibling(2, 2, x);
  CHECK(x_invalid_col.isValid() == false);
  CHECK(x != x_invalid_col);
}

TEST_CASE("adaptor sibling from other col", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><x>...</x><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  QModelIndex root;
  auto doc_root = a.index(0, 0, root);
  auto x = a.index(1, 1, doc_root);
  auto x_left = a.sibling(0, 0, x);
  CHECK(a.data(x_left).toString().toUtf8().data() == string("foo"));
}

TEST_CASE("adaptor sibling loop", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root>"
      "<a>0</a>"
      "<a>1</a>"
      "<a>2</a>"
      "<a>3</a>"
      "<a>4</a>"
      "<a>5</a>"
      "<a>6</a>"
      "</root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  QModelIndex root;
  auto doc_root = a.index(0, 0, root);
  CHECK(a.rowCount(doc_root) == 7);
  int i = 0;
  auto x = a.index(i, 0, doc_root);
  for (; i < a.rowCount(doc_root); ++i) {
    x = a.sibling(i, 1, x);
    CHECK(a.data(x).toInt() == i);
  }
}

TEST_CASE("adaptor sibling loop dec", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root>"
      "<a>0</a>"
      "<a>1</a>"
      "<a>2</a>"
      "<a>3</a>"
      "<a>4</a>"
      "<a>5</a>"
      "<a>6</a>"
      "</root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  QModelIndex root;
  auto doc_root = a.index(0, 0, root);
  CHECK(a.rowCount(doc_root) == 7);
  int i = a.rowCount(doc_root)-1;
  auto x = a.index(i, 0, doc_root);
  for (; i >= 0; --i) {
    x = a.sibling(i, 1, x);
    CHECK(a.data(x).toInt() == i);
  }
}

TEST_CASE("adaptor sibling loop from mid", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root>"
      "<a>0</a>"
      "<a>1</a>"
      "<a>2</a>"
      "<a>3</a>"
      "<a>4</a>"
      "<a>5</a>"
      "<a>6</a>"
      "</root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);

  QModelIndex root;
  auto doc_root = a.index(0, 0, root);
  CHECK(a.rowCount(doc_root) == 7);
  int i = 0;
  auto x = a.index(3, 0, doc_root);
  for (; i < a.rowCount(doc_root); ++i) {
    auto y = a.sibling(i, 1, x);
    CHECK(a.data(y).toInt() == i);
  }
}

TEST_CASE("ignore setting the same value", "[adaptor][xml][tree-model]" )
{
  xxxml::doc::Ptr doc = xxxml::read_memory(
      "<root><foo>Hello</foo><bar>World</bar></root>");

  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor a(m);
  QSignalSpy spy_about(&a, SIGNAL(dataAboutToBeChanged(const QModelIndex&,
          const QModelIndex&, const QVector<int> &)));
  QSignalSpy spy_change(&a, SIGNAL(dataChanged(const QModelIndex&,
          const QModelIndex&, const QVector<int> &)));

  CHECK(a.data(a.index(0, 0).child(0, 1)).toString().toStdString() == "Hello");
  bool r = a.setData(a.index(0, 0).child(0, 1), QVariant(QString("Hello")));
  CHECK(r == false);
  CHECK(spy_about.empty());
  CHECK(spy_change.empty());
}
