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

#include "preview.hh"

#include <ixxx/util.hh>

#include <xfsx/detector.hh>
#include <xfsx/ber2xml.hh>
#include <xfsx/byte.hh>

#include <stdexcept>
#include <string>
#include <deque>

using namespace std;

namespace editor {
  namespace command {

    QString preview(const QString &filename)
    {
      if (filename.endsWith(".xml") || filename.endsWith(".XML"))
        return QString();
      try {
        string asn_config_filename;
        deque<string> asn_search_path;
        string in_filename(filename.toStdString());
        auto r = xfsx::detector::detect_ber(in_filename,
            // empty values -> defaults are used
            asn_config_filename, asn_search_path);

        ixxx::util::Mapped_File in(in_filename);
        xfsx::xml::Pretty_Writer_Arguments args(r.asn_filenames);

        xfsx::byte::writer::Memory m;
        args.count = 18;
        xfsx::xml::pretty_write(in.begin(), in.end(), m, args);
        // XXX also print the ACI
        
        QString s(QString::fromUtf8(m.begin(), m.end()-m.begin()));
        return s;
      } catch (const range_error &e ) {
      }
      return QString();
    }

  } // command
} // editor
