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

#ifndef EDITOR_COMMAND_SEARCH_HH
#define EDITOR_COMMAND_SEARCH_HH

#include <QObject>
#include <QAtomicInt>
#include <QMutex>

#include <deque>
#include <vector>

class QPersistentModelIndex;
class QAbstractItemModel;

struct _xmlNode;
typedef struct _xmlNode xmlNode;

namespace tree_model {
  class Base;
}

namespace editor {
  namespace command {

    class Search : public QObject {
      Q_OBJECT
      public:
        using QObject::QObject;
        ~Search();

        QAtomicInt abort;
        void move_result_to(std::deque<const xmlNode*> &r);

        void set_delay(unsigned d);
        void set_epoche(size_t e);

        static std::vector<QPersistentModelIndex> mk_qpmi_vector(
            const std::deque<const xmlNode*> &xs,
            const QAbstractItemModel &model);

      signals:
        void aborted();
        void finished();

      public slots:
        void set_tree_model(const tree_model::Base *model);
        void search(const QString &key, const QString &value);
        void search_key(const QString &key);
      private:
        QMutex result_mutex_;
        std::deque<const xmlNode*> result_;

        const tree_model::Base *model_ {nullptr};
        size_t epoche_ {1000};
        size_t limit_ {200};
        unsigned delay_ {0};

        void search_prime(const char *key, const char *value);
        void set_result(std::deque<const xmlNode*> &&r);
    };

  } // command
} // editor

#endif // EDITOR_COMMAND_SEARCH_HH
