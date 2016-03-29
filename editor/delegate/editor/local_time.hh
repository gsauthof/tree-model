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

#ifndef EDITOR_DELEGATE_LOCAL_TIME_EDITOR_HH
#define EDITOR_DELEGATE_LOCAL_TIME_EDITOR_HH

#include <QDateTimeEdit>

#include <editor/delegate/value/local_time.hh>

namespace editor {
  namespace delegate {
    namespace editor {

      class Local_Time : public QDateTimeEdit {
        Q_OBJECT
        Q_PROPERTY(::editor::delegate::value::Local_Time local_time
            READ local_time WRITE set_local_time USER true)
        public:
          Local_Time(QWidget *parent = nullptr);

          ::editor::delegate::value::Local_Time local_time() const;
          void set_local_time(
              const ::editor::delegate::value::Local_Time &local_time);
        private:
      };
    } // editor
  } // delegate
} // editor


#endif // EDITOR_DELEGATE_LOCAL_TIME_EDITOR_HH
