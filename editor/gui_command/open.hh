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
#ifndef EDITOR_GUI_COMMAND_OPEN_HH
#define EDITOR_GUI_COMMAND_OPEN_HH

#include <QAbstractItemModel>
#include <QObject>
#include <QList>
#include <QUrl>

#include <functional>

#include <editor/command/async_open.hh>

class QTimer;
class QProgressDialog;
class QWidget;

namespace editor {

  class File_Type;

  namespace gui_command {

    class Open : public editor::command::Async_Open {
      Q_OBJECT
      public:
        explicit Open(QWidget *parent = nullptr);

      public slots:
        void open(const QString &filename);

        void open_urls(const QList<QUrl> &urls);

        // we don't overload because we can't use the
        // new signal/slot syntax then ...
        void open_ft(const QString &filename, const File_Type &ft);

        void set_modified(bool b);

      signals:
        void open_more_urls_requested(const QList<QUrl> &urls);

      protected:
        QWidget *parent_widget_{nullptr};
      private:
        bool modified_ {false};
        void open_prime(const QString &filename, std::function<void()>);
    };

  } // namespace command
} // namespace editor

#endif // EDITOR_COMMAND_OPEN_HH
