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

#include "read_grammar.hh"

#include <editor/file_type.hh>
#include <xfsx/tap.hh>
#include <grammar/grammar.hh>

using namespace std;

namespace editor {
  namespace command {

      Read_Grammar::~Read_Grammar() =default;

      void Read_Grammar::set_file_type(const File_Type &ft)
      {
        if (ft.asn_filenames().empty()) {
          grammar_.reset();
        } else {
          grammar_ = make_unique<grammar::Grammar>(
              xfsx::tap::read_asn_grammar(ft.asn_filenames()));
        }
        emit grammar_read(grammar_.get());
      }

  } // command
} // editor
