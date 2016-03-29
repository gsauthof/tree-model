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

#include <editor/typed_model.hh>

#include <ixxx/util.hh>
#include <ixxx/ixxx.h>

#include <xfsx/ber2lxml.hh>
#include <xfsx/xml_writer_arguments.hh>
#include <xfsx/detector.hh>

#include <deque>

using namespace std;

namespace editor {
  namespace command {

    std::tuple<QAbstractItemModel *, tree_model::Base*, std::deque<string> >
      open_ber(const QString &filename)
    {
      string in_filename(filename.toStdString());
      // empty values -> defaults are used
      string asn_config_filename;
      deque<string> asn_search_path;

      auto r = xfsx::detector::detect_ber(in_filename,
          asn_config_filename, asn_search_path);
      deque<string> asn_filenames(r.asn_filenames);
      xfsx::xml::Pretty_Writer_Arguments pretty_args(asn_filenames);
      pretty_args.dump_indefinite = true;

      ixxx::util::Mapped_File in(in_filename);
      xxxml::doc::Ptr doc = xfsx::xml::l2::generate_tree(in.begin(), in.end(),
          pretty_args);

      //tree_model::Base   *m = new tree_model::XML(std::move(doc));
      auto m = new editor::Typed_Model(std::move(doc));
      m->set_asn_filenames(asn_filenames);
      QAbstractItemModel *a = new tree_model::Item_Adaptor(m);
      return std::make_tuple(a, m, std::move(asn_filenames));
    }

  }
}
