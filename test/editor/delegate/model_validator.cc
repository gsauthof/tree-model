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

#include <QStringListModel>

#include <editor/delegate/model_validator.hh>

#include <string>

using namespace std;

TEST_CASE("validator basic", "[editor][delegate][validator]" )
{
  QStringListModel model;
  QStringList list;
  list << "foo" << "bar" << "foobar";
  list.sort();
  model.setStringList(list);

  editor::delegate::Model_Validator v(model);

  QString s("foo");
  int cursor_pos = 0;
  CHECK(v.validate(s, cursor_pos) == QValidator::Acceptable);

  s = "fubar";
  CHECK(v.validate(s, cursor_pos) == QValidator::Invalid);

  s = "foob";
  CHECK(v.validate(s, cursor_pos) == QValidator::Intermediate);

  v.fixup(s);
  CHECK(s.toStdString() == "foobar");

  s = "foo";
  v.fixup(s);
  CHECK(s.toStdString() == "foo");
}

TEST_CASE("validator empty fixup", "[editor][delegate][validator]")
{
  QStringListModel model;
  QStringList list;
  list << "foo" << "bar" << "foobar";
  list.sort();
  model.setStringList(list);

  editor::delegate::Model_Validator v(model);

  QString s;
  v.fixup(s);
  CHECK(s.toStdString() == "");
}

TEST_CASE("validator ambiguous  fixup", "[editor][delegate][validator]")
{
  QStringListModel model;
  QStringList list;
  list << "foo" << "bar" << "foobar" << "foobarz";
  list.sort();
  model.setStringList(list);

  editor::delegate::Model_Validator v(model);

  QString s("foob");
  v.fixup(s);
  CHECK(s.toStdString() == "foob");
}

