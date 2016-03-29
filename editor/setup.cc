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

#include "setup.hh"

#include "arguments.hh"
#include "file_type.hh"
#include <editor/delegate/value/local_time.hh>
#include <editor/delegate/factory.hh>
#include "qt_awesome.hh"

#include <stdexcept>

using namespace std;

namespace editor {

  class Setup::Private {
    public:
      Private();
    private:
      unique_ptr<QtAwesome> fai;
  };

  Setup::Private::Private()
    :
      fai(editor::fa_instance())
  {
  }

  Setup::Setup()
  {
    static unsigned cnt = 0;
    if (cnt)
      throw logic_error("Setup should only exist once, ideally in main()");
    ++cnt;

    editor::File_Type::register_meta_type();
    editor::delegate::value::Local_Time::register_meta_type();
    editor::delegate::factory::setup();

    editor::setup_asn1_path();
  }

  Setup::~Setup() =default;

}
