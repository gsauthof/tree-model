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

#include <editor/command/open.hh>
#include <editor/command/save.hh>
#include <tree_model/base.hh>

#include <QAbstractItemModel>
#include <QSignalSpy>

#include <boost/filesystem.hpp>

TEST_CASE("save as changes filename", "[editor][qt][save]")
{
  std::string in(test::path::in() + "/small.xml");
  std::string out_a(test::path::out() + "/save_a.xml");
  std::string out_b(test::path::out() + "/save_b.xml");
  boost::filesystem::create_directories(test::path::out());
  boost::filesystem::remove(out_a);
  boost::filesystem::remove(out_b);
  boost::filesystem::copy(in, out_a);

  editor::command::Open o;
  editor::command::Save s;
  QSignalSpy spy_saved(&s, SIGNAL(saved(const QString &)));
  QSignalSpy spy_finished(&s, SIGNAL(finished()));
  QAbstractItemModel *m = nullptr;

  o.connect(&o, &editor::command::Open::tree_model_created,
      &s, &editor::command::Save::set_tree_model);
  o.connect(&o, &editor::command::Open::item_tree_model_created,
      [&m](auto x) { m = x; });
  o.connect(&o, &editor::command::Open::file_opened,
      &s, &editor::command::Save::set_filename);

  o.open(out_a.c_str());

  CHECK(s.filename().endsWith("save_a.xml"));

  s.save_as(out_b.c_str());

  CHECK(s.filename().endsWith("save_b.xml"));
  CHECK(boost::filesystem::exists(out_b));

  REQUIRE(spy_saved.size() == 1);
  CHECK(spy_saved.front().front().toString().endsWith("save_b.xml"));
  CHECK(spy_finished.size() == 1);

  boost::filesystem::remove(out_a);
  s.save();
  CHECK(!boost::filesystem::exists(out_a));

  boost::filesystem::remove(out_b);
  s.save();
  CHECK(!boost::filesystem::exists(out_a));
  CHECK(boost::filesystem::exists(out_b));

  delete m;
}

TEST_CASE("save copy as does not change filename", "[editor][qt][save]")
{
  std::string in(test::path::in() + "/small.xml");
  std::string out_a(test::path::out() + "/save_copy_a.xml");
  std::string out_b(test::path::out() + "/save_copy_b.xml");
  boost::filesystem::create_directories(test::path::out());
  boost::filesystem::remove(out_a);
  boost::filesystem::remove(out_b);
  boost::filesystem::copy(in, out_a);

  editor::command::Open o;
  editor::command::Save s;
  QSignalSpy spy_saved(&s, SIGNAL(saved(const QString &)));
  QSignalSpy spy_finished(&s, SIGNAL(finished()));
  QAbstractItemModel *m = nullptr;

  o.connect(&o, &editor::command::Open::tree_model_created,
      &s, &editor::command::Save::set_tree_model);
  o.connect(&o, &editor::command::Open::item_tree_model_created,
      [&m](auto x) { m = x; });
  o.connect(&o, &editor::command::Open::file_opened,
      &s, &editor::command::Save::set_filename);

  o.open(out_a.c_str());

  CHECK(s.filename().endsWith("save_copy_a.xml"));

  s.save_copy_as(out_b.c_str());

  CHECK(s.filename().endsWith("save_copy_a.xml"));
  CHECK(boost::filesystem::exists(out_b));

  CHECK(spy_saved.size() == 0);
  CHECK(spy_finished.size() == 1);

  boost::filesystem::remove(out_a);
  s.save();
  CHECK(boost::filesystem::exists(out_a));

  boost::filesystem::remove(out_b);
  s.save();
  CHECK(boost::filesystem::exists(out_a));
  CHECK(!boost::filesystem::exists(out_b));

  delete m;
}
