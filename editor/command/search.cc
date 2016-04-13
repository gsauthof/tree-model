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

#include "search.hh"

#include <tree_model/xml.hh>
#include <tree_model/item_adaptor.hh>
#include <xxxml/xxxml.hh>
#include <xxxml/util.hh>

#include <QDebug>
#include <QThread>
#include <QMutexLocker>
#include <QPersistentModelIndex>
#include <string.h>


using namespace std;

namespace editor {
  namespace command {

    Search::~Search()
    {
    }

    // before calling this in a worker thread, an adaptor model
    // referencing the tree model must be switched to read only
    void Search::search_prime(const char *key, const char *value)
    {
      std::deque<const xmlNode*> result;
      auto xml_model = dynamic_cast<const tree_model::XML*>(model_);
      if (!model_ || !xml_model) {
        emit aborted();
      } else {
        const xxxml::doc::Ptr &doc = xml_model->doc();
        xxxml::util::DF_Traverser t(doc);
        size_t i = 1;
        for ( ; !t.eot(); t.advance()) {
          if (   (!key   || !strcmp(xxxml::name(*t), key))
              && (!value
                     || (    xxxml::content((*t)->children)
                         && !strcmp(xxxml::content((*t)->children), value)) )
             ) {
            result.push_back(*t);
          }
          // for debugging purposes
          QThread::msleep(delay_);
          if (i++ % epoche_ == 0 && int(abort)) {
            result.clear();
            emit aborted();
            break;
          }
          if (result.size() == limit_)
            break;
        }
      }
      set_result(std::move(result));
      emit finished();
    }
    void Search::set_result(std::deque<const xmlNode*> &&r)
    {
      QMutexLocker l(&result_mutex_);
      result_ = std::move(r);
    }
    void Search::move_result_to(std::deque<const xmlNode*> &r)
    {
      QMutexLocker l(&result_mutex_);
      r = std::move(result_);
      result_.clear();
    }
    void Search::search(const QString &key, const QString &value)
    {
      search_prime(key.toStdString().c_str(), value.toStdString().c_str());
    }
    void Search::search_key(const QString &key)
    {
      search_prime(key.toStdString().c_str(), nullptr);
    }
    void Search::set_tree_model(const tree_model::Base *model)
    {
      model_ = model;
    }
    void Search::set_delay(unsigned d)
    {
      delay_ = d;
    }
    void Search::set_epoche(size_t e)
    {
      epoche_ = e;
    }
    std::vector<QPersistentModelIndex> Search::mk_qpmi_vector(
        const std::deque<const xmlNode*> &xs,
        const QAbstractItemModel &model)
    {
      vector<QPersistentModelIndex> r;
      auto m = dynamic_cast<const tree_model::Item_Adaptor*>(&model);
      if (m) {
        r.reserve(xs.size());
        for (auto &x : xs) {
          auto path = xxxml::util::path(x);
          QModelIndex j;
          for (auto &p : path)
            j = m->index(const_cast<xmlNode*>(p), j);
          if (j.isValid())
            r.push_back(std::move(j));
        }
      }
      return r;
    }


  } // command
} // editor
