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
#include "remove.hh"

#include <QAbstractItemModel>

#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

namespace editor {
  namespace command {

    Remove::Remove(QObject *parent)
      :
        QObject(parent)
    {
    }

    void remove(QAbstractItemModel *model_, const QModelIndexList &list)
    {
      if (!model_ || list.empty())
        return;

      vector<QModelIndex> v(list.begin(), list.end());
      sort(v.begin(), v.end());
      v.erase(unique(v.begin(), v.end(),
            [](const QModelIndex &a, const QModelIndex &b) {
            return make_pair(a.internalId(), a.row())
                == make_pair(b.internalId(), b.row());  }), v.end());

      if (v.size() == 1) {
        model_->removeRow(v.front().row(), v.front().parent());
      } else {
        vector<QPersistentModelIndex> w;
        w.reserve(v.size());
        for (auto &x : v)
          w.emplace_back(x);
        for (auto &x : w)
          if (x.isValid())
            model_->removeRow(x.row(), x.parent());
      }
    }

    void Remove::remove(const QModelIndexList &list)
    {
      editor::command::remove(model_, list);
    }

    void Remove::set_model(QAbstractItemModel *model)
    {
      model_ = model;
    }

  }
}
