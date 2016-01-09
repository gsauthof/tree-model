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
#include "transaction.hh"


namespace tree_model {
  namespace operation {

    Transaction::Transaction() =default;
    Transaction::Transaction(const QString &title)
      :
        title_(title)
    {
    }

    void Transaction::forward(QAbstractItemModel &model)
    {
      begin_forward();
      for (auto &operation : operations_)
        operation->forward(model);
    }
    void Transaction::rewind(QAbstractItemModel &model)
    {
      begin_rewind();
      for (auto i = operations_.rbegin(); i != operations_.rend(); ++i)
        (*i)->rewind(model);
    }
    void Transaction::commit()
    {
      begin_finalize();
    }
    bool Transaction::empty() const
    {
      return operations_.empty();
    }
    size_t Transaction::size() const
    {
      return operations_.size();
    }
    void Transaction::push(std::unique_ptr<operation::Base> &&op)
    {
      operations_.push_back(std::move(op));
    }

  }
}
