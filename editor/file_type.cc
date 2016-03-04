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

#include "file_type.hh"

namespace editor {

  File_Type::File_Type() =default;
  File_Type::File_Type(Major major) : major_(major) {}
  File_Type::File_Type(Major major, Minor minor)
    : major_(major), minor_(minor) {}

  File_Type::Major File_Type::major() const { return major_; }
  File_Type::Minor File_Type::minor() const { return minor_; }
  const std::deque<std::string> &File_Type::asn_filenames() const
  {
    return asn_filenames_;
  }
  void File_Type::set_asn_filenames(std::deque<std::string> &&asn_filenames)
  {
    asn_filenames_ = std::move(asn_filenames);
  }

  void File_Type::register_meta_type()
  {
    qRegisterMetaType<File_Type>();
    qRegisterMetaType<File_Type>("File_Type");
  }

}
