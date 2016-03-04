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

#include "save_ber.hh"

#include <xfsx/lxml2ber.hh>
#include <xfsx/tap.hh>

#include <tree_model/xml.hh>

using namespace std;

namespace editor {
  namespace command {

    void save_ber(const tree_model::Base &m, const QString &filename,
        const deque<string> &asn_filenames)
    {
      auto x = dynamic_cast<const tree_model::XML*>(&m);
      if (!x)
        throw logic_error("not an xml model");
      xfsx::BER_Writer_Arguments args;
      xfsx::tap::apply_grammar(asn_filenames, args);
      xfsx::xml::l2::write_ber(x->doc(), filename.toStdString(), args);
    }

  } // command
} // editor
