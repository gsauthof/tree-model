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

#ifndef EDITOR_TYPED_MODEL_HH
#define EDITOR_TYPED_MODEL_HH

#include <tree_model/xml.hh>
#include <xfsx/xfsx.hh>

#include <deque>
#include <string>

namespace editor {

  class Typed_Model : public tree_model::XML {
    public:
      using tree_model::XML::XML;

      QVariant data(const tree_model::Index &index,
          int role = Qt::DisplayRole) const override;

      void set_asn_filenames(const std::deque<std::string> &asn_filenames);
    private:
      std::deque<std::string> asn_filenames_;
      xfsx::Tag_Dereferencer tag_dereferencer_;
      xfsx::Name_Translator name_translator_;
  };


} // editor

#endif // EDITOR_TYPED_MODEL_HH
