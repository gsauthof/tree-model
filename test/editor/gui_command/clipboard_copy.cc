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

#include <editor/gui_command/clipboard_copy.hh>
#include <tree_model/item_adaptor.hh>
#include <tree_model/xml.hh>

using namespace std;

TEST_CASE("basic copy clipboard", "[editor][qt][gui][clipboard]")
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

  editor::gui_command::Clipboard_Copy cc;
  cc.set_model(a);

  auto i = a->index(0, 0, QModelIndex()).child(0, 0).child(4, 0);

  cc.copy(QModelIndexList() << i);

  const char ref[] = R"(<TransferCutOffTimeStamp>
            <LocalTimeStamp>20050405090547</LocalTimeStamp>
            <UtcTimeOffset>+0200</UtcTimeOffset>
        </TransferCutOffTimeStamp>)";

  CHECK(cb->text().toStdString() == ref);
#if (defined(__MINGW32__) || defined(__MINGW64__))
  // windows doesn't have primary selection ...
#else
  CHECK(cb->text(QClipboard::Selection).toStdString() == ref);
#endif
  CHECK(cb->mimeData()->data("text/xml").data() == string(ref));

  cb->clear();
  cb->clear(QClipboard::Selection);
  delete a;

}

