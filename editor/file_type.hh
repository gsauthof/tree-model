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

#ifndef EDITOR_FILE_TYPE_HH
#define EDITOR_FILE_TYPE_HH

#include <QMetaType>

#include <deque>
#include <string>

// work around glibc bug, cf.
// http://stackoverflow.com/questions/22240973/major-and-minor-macros-defined-in-sys-sysmacros-h-pulled-in-by-iterator
#ifdef minor
#undef minor
#endif
#ifdef major
#undef major
#endif

namespace editor {

  class File_Type {
    public:
      enum Major { ANY, XML, BER };
      enum Minor { ANY_MINOR, TAP, RAP, NRT };

      File_Type();
      File_Type(Major major);
      File_Type(Major major, Minor minor);

      Major major() const;
      Minor minor() const;
      const std::deque<std::string> &asn_filenames() const;
      void set_asn_filenames(std::deque<std::string> &&asn_filenames);

      // call this if signals having File_Type arguments
      // should be able to be queued
      static void register_meta_type();

    private:
      Major major_ {ANY};
      Minor minor_ {ANY_MINOR};
      std::deque<std::string> asn_filenames_;

  };

} // editor

Q_DECLARE_METATYPE(editor::File_Type)

#endif
