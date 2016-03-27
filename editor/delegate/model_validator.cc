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

#include "model_validator.hh"

#include <tree_model/qmi_iterator.hh>

#include <QAbstractItemModel>

#include <algorithm>
#include <utility>

using namespace std;

namespace editor {
  namespace delegate {

    Model_Validator::Model_Validator(const QAbstractItemModel &model,
        QObject *parent)
      :
        QValidator(parent),
        model_(model)
    {
    }

    static pair<tree_model::QMI_Iterator, tree_model::QMI_Iterator>
    prefix_range(const QAbstractItemModel &model_, const QString &inp)
    {
      auto b = tree_model::QMI_Iterator(model_.index(0, 0));
      auto e = tree_model::QMI_Iterator(model_.index(0, 0),
          model_.rowCount(QModelIndex()));
      auto l = lower_bound(b, e, inp, [](const QVariant &x, const QString &i) {
          auto t = x.toString().left(i.size());
          return t < i;
          } );
      auto u = upper_bound(b, e, inp, [](const QString &i, const QVariant &x) {
          auto t = x.toString().left(i.size());
          return i < t;
          } );
      return make_pair(std::move(l), std::move(u));
    }

    QValidator::State Model_Validator::validate(QString &inp, int &pos) const
    {
      auto p = prefix_range(model_, inp);
      if (p.first < p.second) {
        if ((*p.first).toString() == inp)
          return Acceptable;
        else
          return Intermediate;
      } else {
        return Invalid;
      }
    }

    void Model_Validator::fixup(QString &input) const
    {
      auto p = prefix_range(model_, input);
      if (p.second - p.first == 1 && (*p.first).toString() != input)
        input = (*p.first).toString();
    }

  } // delegate
} // editor
