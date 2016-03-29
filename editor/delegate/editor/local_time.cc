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

#include "local_time.hh"

#include <QDebug>

namespace editor {
  namespace delegate {
    namespace editor {

      Local_Time::Local_Time(QWidget *parent)
        :
          QDateTimeEdit(parent)
      {
        setDisplayFormat("yyyy-MM-dd hh:mm:ss");
        setCalendarPopup(true);
      }

      ::editor::delegate::value::Local_Time Local_Time::local_time() const
      {
        return delegate::value::Local_Time(
            dateTime().toString("yyyyMMddhhmmss"));
      }
      void Local_Time::set_local_time(
          const ::editor::delegate::value::Local_Time &local_time)
      {
        QString s(local_time.to_string());
        setDateTime(QDateTime::fromString(s, "yyyyMMddhhmmss"));
      }

    } // editor
  } // delegate
} // editor
