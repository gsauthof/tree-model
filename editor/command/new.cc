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
#include "new.hh"

#include <tree_model/item_adaptor.hh>
#include <tree_model/xml.hh>

namespace editor {
  namespace command {

    void New::create()
    {
      xxxml::doc::Ptr doc = xxxml::new_doc();
      tree_model::XML *m = new tree_model::XML(std::move(doc));
      tree_model::Item_Adaptor *a = new tree_model::Item_Adaptor(m);
      emit item_tree_model_created(a);
      emit tree_model_created(m);
    }

  } // command
} //editor
