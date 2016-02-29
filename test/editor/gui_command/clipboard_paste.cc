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

#include <QtTest/QtTest>

#include <QAbstractItemModel>
#include <QClipboard>
#include <QItemSelectionModel>
#include <QMimeData>
#include <QApplication>
#include <string>

#include <editor/gui_command/clipboard_paste.hh>
#include <tree_model/item_adaptor.hh>
#include <tree_model/xml.hh>

using namespace std;

TEST_CASE("basic paste clipboard", "[editor][qt][gui][clipboard]")
{
  QClipboard *cb = QApplication::clipboard();
  cb->clear();
  auto d = new QMimeData();
  d->setData("text/xml", QByteArray("<root><a>Hello</a><b>World</b></root>"));
  cb->setMimeData(d);

  tree_model::XML *m = new tree_model::XML(xxxml::new_doc());
  tree_model::Item_Adaptor *a = new tree_model::Item_Adaptor(m);
  editor::gui_command::Clipboard_Paste cp;
  cp.set_model(a);
  auto sm = new QItemSelectionModel(a);
  cp.set_selection_model(sm);

  cp.paste();

  auto doc = a->mimeData(QModelIndexList() << QModelIndex());

  const char ref[] = R"(<?xml version="1.0"?>
<root>
  <a>Hello</a>
  <b>World</b>
</root>
)";
  CHECK(doc->data("text/xml").toStdString() == ref);

  cb->clear();

  delete sm;
  delete a;
  delete doc;
}

TEST_CASE("paste in between clipboard", "[editor][qt][gui][clipboard]")
{
  QClipboard *cb = QApplication::clipboard();
  cb->clear();
  auto d = new QMimeData();
  d->setData("text/xml", QByteArray("<list><element>23</element></list>"));
  cb->setMimeData(d);

  tree_model::XML *m = new tree_model::XML(xxxml::read_file(
        test::path::in() + "/small.xml"));
  tree_model::Item_Adaptor *a = new tree_model::Item_Adaptor(m);
  editor::gui_command::Clipboard_Paste cp;
  cp.set_model(a);
  auto sm = new QItemSelectionModel(a);
  cp.set_selection_model(sm);
  sm->select(a->index(0, 0).child(1, 0), QItemSelectionModel::Select);

  cp.paste();

  auto doc = a->mimeData(QModelIndexList() << QModelIndex());

  const char ref[] = R"(<?xml version="1.0"?>
<root>
  <foo>Hello</foo>
  <list><element>23</element></list><bar>World</bar>
</root>
)";
  CHECK(doc->data("text/xml").toStdString() == ref);

  cb->clear();

  delete sm;
  delete a;
  delete doc;
}

TEST_CASE("paste child clipboard", "[editor][qt][gui][clipboard]")
{
  QClipboard *cb = QApplication::clipboard();
  cb->clear();
  auto d = new QMimeData();
  d->setData("text/xml", QByteArray("<list><element>23</element></list>"));
  cb->setMimeData(d);

  tree_model::XML *m = new tree_model::XML(xxxml::read_file(
        test::path::in() + "/small.xml"));
  tree_model::Item_Adaptor *a = new tree_model::Item_Adaptor(m);
  editor::gui_command::Clipboard_Paste cp;
  cp.set_model(a);
  auto sm = new QItemSelectionModel(a);
  cp.set_selection_model(sm);
  sm->select(a->index(0, 0), QItemSelectionModel::Select);

  cp.paste_as_child();

  auto doc = a->mimeData(QModelIndexList() << QModelIndex());

  const char ref[] = R"(<?xml version="1.0"?>
<root>
  <foo>Hello</foo>
  <bar>World</bar>
<list><element>23</element></list></root>
)";
  CHECK(doc->data("text/xml").toStdString() == ref);

  cb->clear();

  delete sm;
  delete a;
  delete doc;
}

TEST_CASE("paste malformed", "[editor][qt][gui][clipboard]")
{
  QClipboard *cb = QApplication::clipboard();
  cb->clear();
  auto d = new QMimeData();
  d->setData("text/xml", QByteArray("<list><element>23</list>"));
  cb->setMimeData(d);

  tree_model::XML *m = new tree_model::XML(xxxml::read_file(
        test::path::in() + "/small.xml"));
  tree_model::Item_Adaptor *a = new tree_model::Item_Adaptor(m);
  editor::gui_command::Clipboard_Paste cp;
  cp.set_model(a);
  auto sm = new QItemSelectionModel(a);
  cp.set_selection_model(sm);
  sm->select(a->index(0, 0), QItemSelectionModel::Select);

  cp.paste_as_child();

  auto doc = a->mimeData(QModelIndexList() << QModelIndex());

  const char ref[] = R"(<?xml version="1.0"?>
<root>
  <foo>Hello</foo>
  <bar>World</bar>
</root>
)";
  CHECK(doc->data("text/xml").toStdString() == ref);

  cb->clear();

  delete sm;
  delete a;
  delete doc;
}

TEST_CASE("paste two siblings", "[editor][qt][gui][clipboard]")
{
  QClipboard *cb = QApplication::clipboard();
  cb->clear();
  auto d = new QMimeData();
  d->setData("text/xml", QByteArray("<x>1</x><x>2</x>"));
  cb->setMimeData(d);

  tree_model::XML *m = new tree_model::XML(xxxml::read_file(
        test::path::in() + "/small.xml"));
  tree_model::Item_Adaptor *a = new tree_model::Item_Adaptor(m);
  editor::gui_command::Clipboard_Paste cp;
  cp.set_model(a);
  auto sm = new QItemSelectionModel(a);
  cp.set_selection_model(sm);
  sm->select(a->index(0, 0), QItemSelectionModel::Select);

  cp.paste_as_child();

  auto doc = a->mimeData(QModelIndexList() << QModelIndex());

  const char ref[] = R"(<?xml version="1.0"?>
<root>
  <foo>Hello</foo>
  <bar>World</bar>
<x>1</x><x>2</x></root>
)";
  CHECK(doc->data("text/xml").toStdString() == ref);

  cb->clear();

  delete sm;
  delete a;
  delete doc;
}

