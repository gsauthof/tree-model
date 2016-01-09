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
#include "base.hh"

#include <QString>

#include <memory>
#include <deque>

namespace tree_model {
  namespace operation {
    class Transaction : public Base {
      public:
        Transaction();
        Transaction(const QString &title);
        void forward(QAbstractItemModel &model) override;
        void rewind(QAbstractItemModel &model) override;
        void commit();

        size_t size() const;
        bool empty() const;
        void push(std::unique_ptr<operation::Base> &&op);
      private:
        QString title_;
        std::deque<std::unique_ptr<operation::Base> > operations_;
    };
  }
}
