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

namespace editor {
  namespace command {

    std::pair<QAbstractItemModel *, tree_model::Base*>
      open_xml(const QString &filename)
    {
      xxxml::doc::Ptr doc = xxxml::read_file(filename.toUtf8().data());

      tree_model::Base *m = new tree_model::XML(std::move(doc));
      // We don't call moveToThread() on m, because m is
      // getting adopted by a, and moveThread() works
      // recursively.
      //m->moveToThread(QApplication::instance()->thread());
      QAbstractItemModel *a = new tree_model::Item_Adaptor(m);
      return std::make_pair(a, m);
    }

  }
}
