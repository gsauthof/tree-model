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

#include "df_qmi_traverser.hh"

#include <stdexcept>

using namespace std;

namespace tree_model {

  DF_QMI_Traverser::DF_QMI_Traverser(const QModelIndex &root)
    :
      node_(root)
  {
  }
  const QModelIndex &DF_QMI_Traverser::operator*() const
  {
    if (eot())
      throw logic_error("DF_Traverser *: stack is empty");

    return node_;
  }
  void DF_QMI_Traverser::advance()
  {
    if (eot())
      throw logic_error("DF_Traverser advance: stack is empty");

    auto child = node_.child(0, 0);
    if (child.isValid()) {
      node_ = child;
      ++height_;
    } else {
      while (node_.isValid()) {
        auto next = node_.sibling(node_.row()+1, 0);
        if (next.isValid()) {
          node_ = next;
          break;
        }
        node_ = node_.parent();
        --height_;
      }
    }
  }
  void DF_QMI_Traverser::skip_children()
  {
    if (eot())
      throw logic_error("DF_Traverser skip: stack is empty");

    auto next = node_.sibling(node_.row()+1, 0);
    if (next.isValid())
      node_ = next;
    else {
      node_ = node_.parent();
      --height_;
      advance();
    }
  }
  size_t DF_QMI_Traverser::height() const
  {
    if (eot())
      throw logic_error("DF_Traverser skip: stack is empty");

    return height_;
  }
  bool DF_QMI_Traverser::eot() const
  {
    return !node_.isValid();
  }

} // tree_model
