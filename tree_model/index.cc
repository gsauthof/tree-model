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
#include "index.hh"

#include "base.hh"

namespace tree_model {

  Index::Index(const Base *model, unsigned column, void *ptr)
    :
      model_(model),
      column_(column),
      ptr_(ptr)
  {
  }

  Index::Index()
  {
  }

  Index Index::first_child() const
  {
    if (model_)
      return model_->first_child(*this);
    else
      return Index();
  }

  Index Index::last_child() const
  {
    if (model_)
      return model_->last_child(*this);
    else
      return Index();
  }

  unsigned Index::column() const
  {
    return column_;
  }

  QVariant Index::data(int role) const
  {
    if (model_)
      return model_->data(*this, role);
    else
      return QVariant();
  }

  Qt::ItemFlags Index::flags() const
  {
    if (model_)
      return model_->flags(*this);
    else
      return Qt::NoItemFlags;
  }

  qintptr Index::internal_id() const
  {
    return qintptr(ptr_);
  }

  void *Index::internal_pointer() const
  {
    return ptr_;
  }

  bool Index::is_valid() const
  {
    return ptr_;
  }

  const Base *Index::model() const
  {
    return model_;
  }

  Index Index::parent() const
  {
    if (model_)
      return model_->parent(*this);
    else
      return Index();
  }

  Index Index::prev_sibling() const
  {
    if (model_)
      return model_->prev_sibling(*this);
    else
      return Index();
  }

  Index Index::next_sibling() const
  {
    if (model_)
      return model_->next_sibling(*this);
    else
      return Index();
  }

  Index Index::attribute(unsigned column) const
  {
    if (model_)
      return model_->attribute(*this, column);
    else
      return Index();
  }

  bool Index::operator!=(const Index &o) const
  {
    return !(*this == o);
  }

  bool Index::operator<(const Index &o) const
  {
    return ptr_ < o.ptr_ && column_ < o.column_;
  }

  bool Index::operator==(const Index &o) const
  {
    return ptr_ == o.ptr_ && column_ == o.column_;
  }

}
