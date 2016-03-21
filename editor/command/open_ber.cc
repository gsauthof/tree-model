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

#include "open_xml.hh"

#include <tree_model/item_adaptor.hh>
#include <tree_model/xml.hh>

#include <ixxx/util.hh>
#include <ixxx/ixxx.h>

#include <xfsx/ber2lxml.hh>
#include <xfsx/xml_writer_arguments.hh>

#include <deque>

using namespace std;

namespace editor {
  namespace command {

    std::tuple<QAbstractItemModel *, tree_model::Base*, std::deque<string> >
      open_ber(const QString &filename)
    {
      ixxx::util::Mapped_File in(filename.toStdString());
      deque<string> asn_filenames;
      // XXX replace with auto-detection
      string first (ixxx::ansi::getenv("ASN1_PATH"));
      first = first.substr(0, first.find(":"));
      asn_filenames.push_back(first
          + string("/tap_3_12.asn1"));
      xfsx::xml::Pretty_Writer_Arguments pretty_args(asn_filenames);
      pretty_args.dump_indefinite = true;

      xxxml::doc::Ptr doc = xfsx::xml::l2::generate_tree(in.begin(), in.end(),
          pretty_args);

      tree_model::Base   *m = new tree_model::XML(std::move(doc));
      QAbstractItemModel *a = new tree_model::Item_Adaptor(m);
      return std::make_tuple(a, m, std::move(asn_filenames));
    }

  }
}
