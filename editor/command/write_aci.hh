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
#ifndef EDITOR_COMMAND_WRITE_ACI_HH
#define EDITOR_COMMAND_WRITE_ACI_HH

#include <QObject>
#include <QString>

#include <editor/file_type.hh>

#include <functional>

namespace tree_model {
  class Base;
}
class QAbstractItemModel;

namespace editor {

  class File_Type;

  namespace command {

    class Write_ACI : public QObject
    {
      Q_OBJECT
      public:
        using QObject::QObject;

        void set_delay(unsigned d);
        void set_cancel_function(std::function<bool()> cancel_fn);
        void set_epoche(unsigned epoche);
      public slots:
        void set_model(QAbstractItemModel *model);
        void set_tree_model(const tree_model::Base *tree_model);
        void set_file_type(const File_Type &ft);

        void write();

      signals:
        void begin_transaction_requested(const QString &name);
        void commit_requested();
        void msg_produced(const QString &msg);

      private:
        QAbstractItemModel *model_ {nullptr};
        const tree_model::Base *tree_model_ {nullptr};
        File_Type file_type_;

      protected:
        unsigned delay_ {0};
        std::function<bool()> cancel_fn_;
        unsigned epoche_ {500};

    };

  } // namespace command
} // namespace editor

#endif // EDITOR_COMMAND_WRITE_ACI_HH

