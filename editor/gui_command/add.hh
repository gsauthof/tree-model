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
#ifndef EDITOR_GUI_COMMAND_ADD_HH
#define EDITOR_GUI_COMMAND_ADD_HH

#include <QObject>


class QWidget;
class QAbstractItemModel;
class QModelIndex;

namespace editor {
  namespace gui_command {

    class Add : public QObject {
      Q_OBJECT
      public:
        explicit Add(QWidget *parent = nullptr);

      public slots:
        void add_child(const QModelIndex &index);
        void add_sibling(const QModelIndex &index);
        void set_model(QAbstractItemModel *model);

      signals:
        void begin_transaction_requested(const QString &name);
        void commit_requested();

      protected:
        QWidget *parent_widget_ {nullptr};
      private:
        QAbstractItemModel *model_ {nullptr};

        void add_internal(const QModelIndex &index, bool as_child = true);
    };

  } // namespace gui_command
} // namespace editor

#endif
