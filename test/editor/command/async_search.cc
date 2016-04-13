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


#include <editor/command/async_search.hh>
#include <editor/command/search.hh>
#include <editor/command/open_xml.hh>

#include <xxxml/xxxml.hh>

#include <QSignalSpy>
#include <QString>
#include <QAbstractItemModel>
#include <QTest>

#include <string>

using namespace std;

TEST_CASE("async search command", "[editor][search]")
{
  string in {test::path::in() + "/tap_3_12_small.xml"};
  auto r = editor::command::open_xml(in.c_str());
  unique_ptr<QAbstractItemModel> m(r.first);

  editor::command::Async_Search search;
  QSignalSpy spy_abort(&search, SIGNAL(aborted()));
  QSignalSpy spy_finished(&search, SIGNAL(finished()));
  search.set_tree_model(r.second);

  QString key("Imsi");
  QString value("133713371337133");

  search.search(key, value);

  QTest::qWait(300);

  CHECK(spy_abort.empty());
  REQUIRE(spy_finished.size() == 1);

  deque<const xmlNode*> result;
  search.move_result_to(result);
  REQUIRE(result.size() == 2);
  CHECK(xxxml::name(result.at(0)) == string("Imsi"));
}

TEST_CASE("async search abort", "[editor][search]")
{
  string in {test::path::in() + "/tap_3_12_small.xml"};
  auto r = editor::command::open_xml(in.c_str());
  unique_ptr<QAbstractItemModel> m(r.first);

  editor::command::Async_Search search;
  search.set_delay(400);
  search.set_epoche(1);
  QSignalSpy spy_abort(&search, SIGNAL(aborted()));
  QSignalSpy spy_finished(&search, SIGNAL(finished()));
  search.set_tree_model(r.second);

  QString key("Imsi");
  QString value("133713371337133");

  search.search(key, value);

  QTest::qWait(300);
  search.abort();
  QTest::qWait(400);

  CHECK(spy_abort.size() == 1);
  CHECK(spy_finished.size() == 1);
  deque<const xmlNode*> result;
  search.move_result_to(result);
  CHECK(result.empty());
}


