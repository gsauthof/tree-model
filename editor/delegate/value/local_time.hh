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

#ifndef EDITOR_DELEGATE_VALUE_LOCAL_TIME_HH
#define EDITOR_DELEGATE_VALUE_LOCAL_TIME_HH

#include <QString>
#include <QMetaType>

namespace editor { 
  namespace delegate {

    namespace value {

      class Local_Time {
        public:
          Local_Time();
          Local_Time(const QString &s);
          // with const QString & -> compile error
          QString to_string() const;

          static void register_meta_type();
        private:
          QString s_;
      };

    } // value
  } // delegate
} //editor

//Q_DECLARE_METATYPE(editor::delegate::value::Local_Time)
Q_DECLARE_METATYPE(::editor::delegate::value::Local_Time)

#endif // EDITOR_DELEGATE_VALUE_LOCAL_TIME_HH
