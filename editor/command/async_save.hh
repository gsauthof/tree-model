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
#ifndef EDITOR_COMMAND_ASYNC_SAVE_HH
#define EDITOR_COMMAND_ASYNC_SAVE_HH

#include <editor/file_type.hh>

#include <QObject>
class QThread;

namespace tree_model {
  class Base;
}
namespace editor {
  class File_Type;

  namespace command {
    class Save;

    class Async_Save : public QObject {
      Q_OBJECT
      public:
        explicit Async_Save(QObject *parent = nullptr);
        ~Async_Save();
        const QString &filename() const;
        void set_delay(unsigned msecs);

      signals:
        void msg_produced(const QString &msg);
        void failed(const QString &msg);
        void saved(const QString &filename);
        void finished();

        void save_requested();
        void save_as_requested(const QString &);
        void save_copy_as_requested(const QString &);

      public slots:
        void save();
        void save_as(const QString &filename);
        void save_copy_as(const QString &filename);

        void set_tree_model(tree_model::Base *model);
        void set_filename(const QString &filename);
        void set_file_type(const File_Type &ft);

      protected:
        File_Type file_type_;

      private:
        QThread *thread_ {nullptr};
        Save    *save_   {nullptr};
    };

  }
}

#endif
