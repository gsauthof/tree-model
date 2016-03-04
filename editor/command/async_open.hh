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
#ifndef EDITOR_COMMAND_ASYNC_OPEN_HH
#define EDITOR_COMMAND_ASYNC_OPEN_HH

#include <QObject>
class QThread;
class QAbstractItemModel;

namespace tree_model {
  class Base;
}
namespace editor {

  class File_Type;

  namespace command {
    class Open;

    class Async_Open : public QObject {
      Q_OBJECT
      public:
        explicit Async_Open(QObject *parent = nullptr);
        ~Async_Open();

        void set_delay(unsigned msecs);

      signals:
        void item_tree_model_created(QAbstractItemModel *model);
        void tree_model_created(tree_model::Base *model);
        void failed(const QString &msg);
        void msg_produced(const QString &msg);
        void file_opened(const QString &filename);
        void file_type_opened(const File_Type &ft);
        void finished();

        void open_requested(const QString &filename);
        void open_ft_requested(const QString &filename, const File_Type &ft);

      public slots:
          void open(const QString &filename);
          void open_ft(const QString &filename, const File_Type &ft);
          void ignore_result();

      private:
        QThread *thread_ {nullptr};
        Open    *open_   {nullptr};
        void init();
    };

  } // namespace command
} // namespace editor

#endif // EDITOR_COMMAND_OPEN_HH
