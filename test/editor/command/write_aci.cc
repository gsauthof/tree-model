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

#include <editor/command/open_xml.hh>
#include <editor/command/write_aci.hh>
#include <editor/file_type.hh>

#include <tree_model/util.hh>

#include <memory>
#include <string>
#include <deque>

#include <QSignalSpy>
#include <QAbstractItemModel>

using namespace std;

TEST_CASE("writeaci basic", "[editor][qt][write-aci]")
{
  string in {test::path::in() + "/tap_3_12_small.xml"};
  auto r = editor::command::open_xml(in.c_str());
  unique_ptr<QAbstractItemModel> m(r.first);

  editor::command::Write_ACI waci;

  QSignalSpy spy_begin(&waci,
      SIGNAL(begin_transaction_requested(const QString&)));
  QSignalSpy spy_commit(&waci,
      SIGNAL(commit_requested()));

  waci.set_model(m.get());
  editor::File_Type ft(editor::File_Type::XML);
  deque<string> asn_filenames = { test::path::in()
      + "/../../libgrammar/test/in/asn1/tap_3_12_strip.asn1"  };
  ft.set_asn_filenames(std::move(asn_filenames));
  waci.set_file_type(ft);

  waci.write();

  CHECK(spy_begin.size() == 1);
  CHECK(spy_commit.size() == 1);

  auto root = m->index(0, 0);
  auto aci = tree_model::util::find_child(root, "AuditControlInfo");
  REQUIRE(aci.isValid());

  auto total_charge = tree_model::util::find_child(aci, "TotalCharge");

  REQUIRE(total_charge.isValid());

  CHECK(total_charge.sibling(total_charge.row(), 1).data()
      .toString().toStdString() == "71200");

}
