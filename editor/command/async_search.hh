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

#ifndef EDITOR_COMMAND_ASYNC_SEARCH_HH
#define EDITOR_COMMAND_ASYNC_SEARCH_HH

// must be included because of use in set_tree_mode_requested signal signature
#include <tree_model/base.hh>

#include <QObject>
#include <deque>
#include <vector>

class QThread;
class QString;
class QPersistentModelIndex;
class QAbstractItemModel;

struct _xmlNode;
typedef struct _xmlNode xmlNode;

namespace tree_model {
  class Base;
}

namespace editor {
  namespace command {

    class Search;

    class Async_Search : public QObject {
      Q_OBJECT
      public:
        Async_Search(QObject *parent = nullptr);
        ~Async_Search();

        void set_delay(unsigned d);
        void set_epoche(size_t e);

        void move_result_to(std::deque<const xmlNode*> &r);

        static std::vector<QPersistentModelIndex> mk_qpmi_vector(
            const std::deque<const xmlNode*> &xs,
            const QAbstractItemModel &model);
      signals:
        void aborted();
        void finished();

        void set_tree_model_requested(const tree_model::Base *model);
        void search_requested(const QString &key, const QString &value);
        void search_key_requested(const QString &key);
      public slots:
        void set_tree_model(const tree_model::Base *model);
        void search(const QString &key, const QString &value);
        void search_key(const QString &key);
        void abort();
      private:
        QThread *thread_ {nullptr};
      protected:
        Search  *search_ {nullptr};
    };

  } // command
} // editor


#endif // EDITOR_COMMAND_ASYNC_SEARCH_HH

