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

#include "async_search.hh"


#include <QThread>
#include <QPersistentModelIndex>

#include "search.hh"

namespace editor {
  namespace command {


    Async_Search::Async_Search(QObject *parent)
      :
        QObject(parent),
        thread_(new QThread),
        search_(new Search)
    {
      search_->moveToThread(thread_);
      connect(thread_, &QThread::finished, search_, &QObject::deleteLater);

      connect(search_, &Search::aborted, this, &Async_Search::aborted);
      connect(search_, &Search::finished, this, &Async_Search::finished);

      connect(this, &Async_Search::set_tree_model_requested,
          search_, &Search::set_tree_model);
      connect(this, &Async_Search::search_requested, search_, &Search::search);
      connect(this, &Async_Search::search_key_requested,
          search_, &Search::search_key);

      thread_->start();
    }
    Async_Search::~Async_Search()
    {
      thread_->quit();
      // we don't want to wait forever e.g. on exit, for a pending open ...
      // (otherwise, if we don't wait at all we get a memory leak because
      // deleteLater is not executed)
      if (thread_->wait(100))
        delete thread_;
    }
    void Async_Search::move_result_to(std::deque<const xmlNode*> &r)
    {
      search_->move_result_to(r);
    }
    void Async_Search::set_tree_model(const tree_model::Base *model)
    {
      emit set_tree_model_requested(model);
    }
    void Async_Search::search(const QString &key, const QString &value)
    {
      emit search_requested(key, value);
    }
    void Async_Search::search_key(const QString &key)
    {
      emit search_key_requested(key);
    }
    void Async_Search::abort()
    {
      ++(search_->abort);
    }
    void Async_Search::set_delay(unsigned d)
    {
      search_->set_delay(d);
    }
    void Async_Search::set_epoche(size_t e)
    {
      search_->set_epoche(e);
    }
    std::vector<QPersistentModelIndex> Async_Search::mk_qpmi_vector(
        const std::deque<const xmlNode*> &xs,
        const QAbstractItemModel &model)
    {
      return Search::mk_qpmi_vector(xs, model);
    }


  } // command
} // editor
