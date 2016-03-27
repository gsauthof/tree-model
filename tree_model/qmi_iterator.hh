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

#ifndef TREE_MODEL_QMI_ITERATOR_HH
#define TREE_MODEL_QMI_ITERATOR_HH

#include <QModelIndex>

namespace tree_model {

  class QMI_Iterator {
    public:
      using iterator_category = std::random_access_iterator_tag;
      using value_type = QVariant;
      using reference = QVariant&;
      using pointer = QVariant*;
      using difference_type = int;

      QMI_Iterator(const QModelIndex &i, int off = 0);

      bool operator==(const QMI_Iterator &o) const;
      bool operator!=(const QMI_Iterator &o) const;
      bool operator<(const QMI_Iterator &o) const;
      bool operator<=(const QMI_Iterator &o) const;
      bool operator>(const QMI_Iterator &o) const;
      bool operator>=(const QMI_Iterator &o) const;

      QMI_Iterator &operator++();
      QMI_Iterator operator++(int);
      QMI_Iterator &operator--();
      QMI_Iterator operator--(int);

      QMI_Iterator &operator+=(int n);
      QMI_Iterator &operator-=(int n);

      QMI_Iterator operator+(int n) const;
      QMI_Iterator operator-(int n) const;
      int operator-(const QMI_Iterator &o) const;

      QVariant operator*() const;
      QVariant operator[](int x) const;

    private:
      QModelIndex k_;
      int off_ {0};
  };
  QMI_Iterator operator+(int n, const QMI_Iterator &o);

}

#endif // TREE_MODEL_QMI_ITERATOR_HH
