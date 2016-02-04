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

#include <editor/gui_command/clipboard_cut.hh>
#include <tree_model/item_adaptor.hh>
#include <tree_model/xml.hh>

using namespace std;

TEST_CASE("basic cut clipboard", "[editor][qt][gui][clipboard]")
{
  QClipboard *cb = QApplication::clipboard();
  cb->clear();
  cb->clear(QClipboard::Selection);
  CHECK(cb->text().toStdString() == "");
  CHECK(cb->text(QClipboard::Selection).toStdString() == "");

  std::string in(test::path::in() + "/tap_3_12_small.xml");

  xxxml::doc::Ptr doc = xxxml::read_file(in);
  tree_model::XML *m = new tree_model::XML(std::move(doc));
  tree_model::Item_Adaptor *a = new tree_model::Item_Adaptor(m);

  editor::gui_command::Clipboard_Cut cc;
  cc.set_model(a);
  auto sm = new QItemSelectionModel(a);
  cc.set_selection_model(sm);

  auto old_rowcount = a->rowCount(a->index(0, 0, QModelIndex()).child(0, 0));
  auto old_data = a->data(
      a->index(0, 0, QModelIndex()).child(0, 0).child(4, 0))
    .toString().toStdString();
  auto i = a->index(0, 0, QModelIndex()).child(0, 0).child(4, 0);

  sm->select(i, QItemSelectionModel::Select);

  cc.cut();

  const char ref[] = R"(<TransferCutOffTimeStamp>
            <LocalTimeStamp>20050405090547</LocalTimeStamp>
            <UtcTimeOffset>+0200</UtcTimeOffset>
        </TransferCutOffTimeStamp>)";

  CHECK(cb->text().toStdString() == ref);
  CHECK(cb->text(QClipboard::Selection).toStdString() == ref);
  CHECK(cb->mimeData()->data("text/xml").data() == string(ref));

  auto new_rowcount = a->rowCount(a->index(0, 0, QModelIndex()).child(0, 0));
  CHECK(old_rowcount == new_rowcount+1);
  auto new_data = a->data(
      a->index(0, 0, QModelIndex()).child(0, 0).child(4, 0))
    .toString().toStdString();
  CHECK(old_data != new_data);
  CHECK(old_data == "TransferCutOffTimeStamp");
  CHECK(new_data == "FileAvailableTimeStamp");

  cb->clear();
  cb->clear(QClipboard::Selection);
  delete sm;
  delete a;

}

