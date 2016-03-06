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
#include "clipboard_cut.hh"
#include <editor/command/remove.hh>

#include <stdexcept>

using namespace std;

namespace editor {
  namespace gui_command {

    void Clipboard_Cut::cut(const QModelIndexList &is)
    {
      try {
        copy(is);
        editor::command::remove(model_, is);
      } catch (const underflow_error &e) {
        // do nothing
      }
    }

    // we need it non-const in contrast to the base class
    void Clipboard_Cut::set_model(QAbstractItemModel *model)
    {
      Clipboard_Copy::set_model(model);
      model_ = model;
    }



  }
}
