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

#include <editor/delegate/tag.hh>
#include <editor/command/read_grammar.hh>
#include <editor/file_type.hh>

#include <grammar/grammar.hh>

#include <QStringListModel>
#include <QLineEdit>
#include <QCompleter>

#include <string>
#include <deque>

using namespace std;

TEST_CASE("complete tag", "[editor][gui][complete][delegate]")
{
  // dummy to get some valid index
  QStringListModel m(QStringList() << "foo");

  editor::command::Read_Grammar rg;

  editor::delegate::Tag td;

  rg.connect(&rg, &editor::command::Read_Grammar::grammar_read,
      &td, &editor::delegate::Tag::apply_grammar);

  editor::File_Type ft;
  ft.set_asn_filenames({ test::path::in()
      + "/../../libgrammar/test/in/asn1/tap_3_12_strip.asn1"});
  rg.set_file_type(ft);

  auto i = m.index(0, 0);
  unique_ptr<QWidget> edit(td.createEditor(nullptr, QStyleOptionViewItem(),
      i));
  REQUIRE(edit.get());
  auto line_edit = dynamic_cast<QLineEdit*>(edit.get());
  REQUIRE(line_edit);
  auto c = line_edit->completer();
  REQUIRE(c);

  c->setCompletionPrefix("CallEve");
  deque<string> r;
  for (int i = 0; c->setCurrentRow(i); ++i)
    r.push_back(c->currentCompletion().toStdString());
  REQUIRE(r.size() > 0);
  CHECK(r[0] == "CallEventDetailList");
  REQUIRE(r.size() > 1);
  CHECK(r[1] == "CallEventDetailsCount");
  REQUIRE(r.size() > 2);
  CHECK(r[2] == "CallEventStartTimeStamp");

}

