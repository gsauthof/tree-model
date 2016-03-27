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


/* See also:

http://en.cppreference.com/w/cpp/concept/RandomAccessIterator
http://en.cppreference.com/w/cpp/concept/BidirectionalIterator
http://en.cppreference.com/w/cpp/concept/ForwardIterator
http://en.cppreference.com/w/cpp/language/operators

 */

#include "qmi_iterator.hh"

namespace tree_model {

  QMI_Iterator::QMI_Iterator(const QModelIndex &k, int off)
    :
      k_(k),
      off_(off)
  {
  }

  bool QMI_Iterator::operator==(const QMI_Iterator &o) const
  {
    return k_.row() + off_ == o.k_.row() + o.off_;
  }
  bool QMI_Iterator::operator!=(const QMI_Iterator &o) const
  {
    return !(*this == o);
  }
  bool QMI_Iterator::operator<(const QMI_Iterator &o) const
  {
    return k_.row() + off_ < o.k_.row() + o.off_;
  }
  bool QMI_Iterator::operator<=(const QMI_Iterator &o) const
  {
    return !(*this > o);
  }
  bool QMI_Iterator::operator>(const QMI_Iterator &o) const
  {
    return k_.row() + off_ > o.k_.row() + o.off_;
  }
  bool QMI_Iterator::operator>=(const QMI_Iterator &o) const
  {
    return !(*this < o);
  }
  QMI_Iterator &QMI_Iterator::operator++()
  {
    ++off_;
    return *this;
  }
  QMI_Iterator QMI_Iterator::operator++(int)
  {
    QMI_Iterator a(*this);
    ++(*this);
    return a;
  }
  QMI_Iterator &QMI_Iterator::operator--()
  {
    --off_;
    return *this;
  }
  QMI_Iterator QMI_Iterator::operator--(int)
  {
    QMI_Iterator a(*this);
    --(*this);
    return a;
  }
  QMI_Iterator &QMI_Iterator::operator+=(int n)
  {
    off_ += n;
    return *this;
  }
  QMI_Iterator &QMI_Iterator::operator-=(int  n)
  {
    off_ -= n;
    return *this;
  }
  QMI_Iterator QMI_Iterator::operator+(int n) const
  {
    QMI_Iterator a(*this);
    a += n;
    return a;
  }
  QMI_Iterator operator+(int n, const QMI_Iterator &o)
  {
    return o + n;
  }
  QMI_Iterator QMI_Iterator::operator-(int n) const
  {
    QMI_Iterator a(*this);
    a -= n;
    return a;
  }
  int QMI_Iterator::operator-(const QMI_Iterator &o) const
  {
    return (k_.row() + off_) - (o.k_.row() + o.off_);
  }
  QVariant QMI_Iterator::operator*() const
  {
    return k_.sibling(k_.row() + off_, 0).data();
  }
  QVariant QMI_Iterator::operator[](int n) const
  {
    return *((*this)+n);
  }

} // tree_model

