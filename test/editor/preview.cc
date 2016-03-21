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

#include <editor/preview.hh>
#include <editor/command/preview.hh>

#include <QTextEdit>
#include <QDebug>

TEST_CASE("preview widget", "[editor][gui][preview]")
{
  editor::Preview p;
  p.set_delegate(editor::command::preview);

  std::string in(test::path::in()
      + "/../../libxfsx/test/in/tap_3_12_valid.ber");
  p.show_preview(in.c_str());
  CHECK(p.text_edit().toPlainText().startsWith("<TransferBatch>"));
}

TEST_CASE("preview widget xml", "[editor][gui][preview]")
{
  editor::Preview p;
  p.set_delegate(editor::command::preview);

  std::string in(test::path::in() + "/tap_3_12_small.xml");
  p.show_preview(in.c_str());
  CHECK(p.text_edit().toPlainText().startsWith("<TransferBatch>"));
}

TEST_CASE("preview widget empty", "[editor][gui][preview]")
{
  editor::Preview p;
  p.set_delegate(editor::command::preview);

  std::string in(test::path::in());
  p.show_preview(in.c_str());
  CHECK(p.text_edit().toPlainText().isEmpty());
}
