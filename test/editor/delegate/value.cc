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

#include <editor/delegate/value.hh>
#include <editor/command/read_grammar.hh>
#include <editor/file_type.hh>

#include <tree_model/item_adaptor.hh>
#include <tree_model/xml.hh>

#include <grammar/grammar.hh>

#include <QStringListModel>
#include <QLineEdit>
#include <QCompleter>
#include <QValidator>

#include <string>
#include <deque>

using namespace std;

TEST_CASE("complete value", "[editor][gui][complete][delegate]")
{
  auto a = make_unique<tree_model::Item_Adaptor>(
      new tree_model::XML(xxxml::new_doc()));

  a->insertRows(0, 1);
  a->setData(a->index(0, 0), "CallTypeLevel1");
  a->setData(a->index(0, 1), "");

  editor::delegate::Value vd;
  vd.read_constraints({test::path::in()
      + "/../../libgrammar/grammar/xml/tap_3_12_constraints.zsv"});

  auto edit = vd.createEditor(nullptr, QStyleOptionViewItem(), a->index(0, 1));
  REQUIRE(edit);
  auto line_edit = dynamic_cast<QLineEdit*>(edit);
  REQUIRE(line_edit);
  auto c = line_edit->completer();
  REQUIRE(c);

  c->setCompletionPrefix("10");
  deque<string> r;
  for (int i = 0; c->setCurrentRow(i); ++i)
    r.push_back(c->currentCompletion().toStdString());
  REQUIRE(r.size() > 0);
  CHECK(r[0] == "10");
  REQUIRE(r.size() > 1);
  CHECK(r[1] == "100");
}

TEST_CASE("complete tadig code", "[editor][gui][complete][delegate]")
{
  auto a = make_unique<tree_model::Item_Adaptor>(
      new tree_model::XML(xxxml::new_doc()));

  a->insertRows(0, 1);
  a->setData(a->index(0, 0), "Sender");
  a->setData(a->index(0, 1), "");

  editor::delegate::Value vd;
  editor::File_Type ft;
  ft.set_constraint_filenames({test::path::in()
      + "/../../libgrammar/grammar/xml/tadig_codes.zsv"});
  vd.apply_file_type(ft);

  auto edit = vd.createEditor(nullptr, QStyleOptionViewItem(), a->index(0, 1));
  REQUIRE(edit);
  auto line_edit = dynamic_cast<QLineEdit*>(edit);
  REQUIRE(line_edit);
  auto c = line_edit->completer();
  REQUIRE(c);

  c->setCompletionPrefix("USA");
  deque<string> r;
  for (int i = 0; c->setCurrentRow(i); ++i)
    r.push_back(c->currentCompletion().toStdString());
  REQUIRE(r.size() > 0);
  CHECK(r[0] == "USA16");
  REQUIRE(r.size() > 1);
  CHECK(r[1] == "USA27");
}

TEST_CASE("validate value", "[editor][gui][complete][delegate]")
{
  auto a = make_unique<tree_model::Item_Adaptor>(
      new tree_model::XML(xxxml::new_doc()));

  a->insertRows(0, 1);
  a->setData(a->index(0, 0), "BearerServiceCode");
  a->setData(a->index(0, 1), "");

  editor::delegate::Value vd;
  vd.read_constraints({test::path::in()
      + "/../../libgrammar/grammar/xml/tap_3_12_constraints.zsv"});

  auto edit = vd.createEditor(nullptr, QStyleOptionViewItem(), a->index(0, 1));
  REQUIRE(edit);
  auto line_edit = dynamic_cast<QLineEdit*>(edit);
  REQUIRE(line_edit);
  auto v = line_edit->validator();
  REQUIRE(v);

  int i = 0;
  QString s = "6";

  CHECK(v->validate(s, i) == QValidator::Intermediate);

  s = "60";
  CHECK(v->validate(s, i) == QValidator::Acceptable);

  s = "6B0";
  CHECK(v->validate(s, i) == QValidator::Invalid);
}

TEST_CASE("validate value domain", "[editor][gui][complete][delegate]")
{
  auto a = make_unique<tree_model::Item_Adaptor>(
      new tree_model::XML(xxxml::new_doc()));

  a->insertRows(0, 1);
  a->setData(a->index(0, 0), "ChargedPartyIdType");
  a->setData(a->index(0, 1), "");

  editor::delegate::Value vd;
  vd.read_constraints({test::path::in()
      + "/../../libgrammar/grammar/xml/tap_3_12_constraints.zsv"});

  auto edit = vd.createEditor(nullptr, QStyleOptionViewItem(), a->index(0, 1));
  REQUIRE(edit);
  auto line_edit = dynamic_cast<QLineEdit*>(edit);
  REQUIRE(line_edit);
  auto v = line_edit->validator();
  REQUIRE(v);

  int i = 0;
  QString s = "1";
  CHECK(v->validate(s, i) == QValidator::Acceptable);
  s = "8";
  CHECK(v->validate(s, i) == QValidator::Acceptable);
  s = "0";
  // would be Intermediate with QIntValidator
  CHECK(v->validate(s, i) == QValidator::Invalid);
  s = "01";
  // would be Acceptable with QIntValidator
  CHECK(v->validate(s, i) == QValidator::Invalid);
  s = "9";
  CHECK(v->validate(s, i) == QValidator::Invalid);
  s = "-1";
  CHECK(v->validate(s, i) == QValidator::Invalid);
}

TEST_CASE("validate value enum", "[editor][gui][complete][delegate]")
{
  auto a = make_unique<tree_model::Item_Adaptor>(
      new tree_model::XML(xxxml::new_doc()));

  a->insertRows(0, 1);
  a->setData(a->index(0, 0), "CauseForTerm");
  a->setData(a->index(0, 1), "");

  editor::delegate::Value vd;
  vd.read_constraints({test::path::in()
      + "/../../libgrammar/grammar/xml/tap_3_12_constraints.zsv"});

  auto edit = vd.createEditor(nullptr, QStyleOptionViewItem(), a->index(0, 1));
  REQUIRE(edit);
  auto line_edit = dynamic_cast<QLineEdit*>(edit);
  REQUIRE(line_edit);
  auto v = line_edit->validator();
  REQUIRE(v);

  int i = 0;
  QString s = "3";
  CHECK(v->validate(s, i) == QValidator::Acceptable);
  s = "2";
  CHECK(v->validate(s, i) == QValidator::Intermediate);
  s = "21";
  CHECK(v->validate(s, i) == QValidator::Acceptable);
  s = "211";
  CHECK(v->validate(s, i) == QValidator::Invalid);
  s = "";
  CHECK(v->validate(s, i) == QValidator::Intermediate);
  s = "0";
  CHECK(v->validate(s, i) == QValidator::Invalid);
}

TEST_CASE("validate half specified domain", "[editor][gui][complete][delegate]")
{
  auto a = make_unique<tree_model::Item_Adaptor>(
      new tree_model::XML(xxxml::new_doc()));

  a->insertRows(0, 1);
  a->setData(a->index(0, 0), "CallEventDetailsCount");
  a->setData(a->index(0, 1), "4");

  editor::delegate::Value vd;
  vd.read_constraints({test::path::in()
      + "/../../libgrammar/grammar/xml/tap_3_12_constraints.zsv"});

  auto edit = vd.createEditor(nullptr, QStyleOptionViewItem(), a->index(0, 1));
  REQUIRE(edit);
  auto line_edit = dynamic_cast<QLineEdit*>(edit);
  REQUIRE(line_edit);
  auto v = line_edit->validator();
  REQUIRE(v);
  int i = 0;
  QString s = "0";
  CHECK(v->validate(s, i) == QValidator::Invalid);
  s = "1";
  CHECK(v->validate(s, i) == QValidator::Acceptable);
  s = "10";
  CHECK(v->validate(s, i) == QValidator::Acceptable);
  s = "-1";
  CHECK(v->validate(s, i) == QValidator::Invalid);
  s = "9223372036854775807";
  CHECK(v->validate(s, i) == QValidator::Acceptable);
}

