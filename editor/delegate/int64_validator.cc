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


#include "int64_validator.hh"

namespace editor {
  namespace delegate {

    // we don't use QIntValidator because:
    // - it doesn't support long ints
    // - it allows leading zeros
    // - it allows grouping characters, by default

    Int64_Validator::Int64_Validator(int64_t min, int64_t max, QObject *parent)
      :
        range_(min, max)
    {
    }

    QValidator::State Int64_Validator::validate(QString &input, int &pos) const
    {
      bool ok = false;
      auto x = input.toLongLong(&ok);
      static_assert(sizeof(x) == 8, "should be 64 bit");
      if (!ok)
        return Invalid;
      auto s = QString::number(x);
      if (input != s)
        return Invalid;
      return x >= range_.first && x <= range_.second ? Acceptable : Invalid;
    }

  } // delegate
} // editor
