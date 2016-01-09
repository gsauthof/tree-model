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
#ifndef EDITOR_COMMAND_SAVE_HH
#define EDITOR_COMMAND_SAVE_HH

#include <QObject>
#include <QString>

namespace tree_model {
  class Base;
}

namespace editor {
  namespace command {

    class Save : public QObject {
      Q_OBJECT
      public:
        explicit Save(QObject *parent = nullptr);

        const QString &filename() const;
        void set_delay(unsigned msecs);

      signals:
        void msg_produced(const QString &msg);
        void failed(const QString &msg);
        void saved(const QString &filename);
        void finished();

      public slots:
        void save();
        void save_as(const QString &filename);

        void set_tree_model(tree_model::Base *model);
        void set_filename(const QString &filename);

      private:
        tree_model::Base *model_ {nullptr};
        QString filename_;
        unsigned delay_{0};
    };
  }
}


#endif
