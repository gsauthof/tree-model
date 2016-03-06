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
#include "util.hh"

#include <QDebug>
#include <QAbstractItemModel>

#include <stack>
#include <utility>

using namespace std;

namespace tree_model {

  namespace util {

    bool descendents_less_than(const QAbstractItemModel &m,
        const QModelIndex &i, size_t n)
    {
      stack<QModelIndex> stack;
      if (m.hasChildren(i))
        stack.push(m.index(0, 0, i));

      size_t c = 0;
      while (!stack.empty()) {
        auto top = stack.top();
        stack.pop();
        {
          auto x = m.sibling(top.row()+1, 0, top);
          ++c;
          if (c >= n)
            return false;
          if (x.isValid())
            stack.push(x);
        }
        if (m.hasChildren(top))
          stack.push(m.index(0, 0, top));
      }
      return true;
    }

    void breadth_first(const QModelIndex &root, unsigned n,
        std::function<void(const QModelIndex &)> f)
    {
      auto m = root.model();
      if (!m)
        return;
      if (!root.isValid())
        return;
      deque<QModelIndex> parents;
      parents.push_back(root);
      deque<QModelIndex> future_parents;

      unsigned x = 1;

      while (!parents.empty()) {
        for (auto &p : parents) {
          x += 1 + m->rowCount(p);
          if (x > n)
            return;
          for (auto i = p.child(0, 0); i.isValid(); i = i.sibling(i.row()+1, 0))
            future_parents.push_back(i);
          f(p);
        }
        //parents = std::move(future_parents);
        swap(parents, future_parents);
        future_parents.clear();
      }
    }

  } // util

} // tree_model
