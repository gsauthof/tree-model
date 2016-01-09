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
#ifndef TREE_MODEL_INDEX_HH
#define TREE_MODEL_INDEX_HH

#include <Qt>
#include <QVariant>

namespace tree_model {

  class Base;

  class Index
  {
    private:
      friend class Base;

      const Base *model_ {nullptr};
      unsigned column_ {0};
      void *ptr_ {nullptr};

    protected:
      Index(const Base *model, unsigned column, void *ptr);
    public:
      Index();

      Index first_child() const;
      Index last_child() const;
      unsigned column() const;
      QVariant data(int role = Qt::DisplayRole) const;
      Qt::ItemFlags flags() const;
      qintptr internal_id() const;
      void *internal_pointer() const;
      bool is_valid() const;
      const Base *model() const;
      Index parent() const;
      Index prev_sibling() const;
      Index next_sibling() const;
      Index attribute(unsigned column) const;
      bool operator!=(const Index &o) const;
      bool operator<(const Index &o) const;
      bool operator==(const Index &o) const;
  };

}

Q_DECLARE_METATYPE(tree_model::Index)


#endif // TREE_MODEL_INDEX_HH
