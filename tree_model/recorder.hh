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
#ifndef TREE_MODEL_RECORDER_HH
#define TREE_MODEL_RECORDER_HH

#include <QObject>

#include <deque>
#include <memory>
#include <limits>

class QAbstractItemModel;

namespace tree_model {

  namespace operation {
    class Base;
    class Transaction;
    class Set_Data;
    class Remove_Rows;
  }

  class Recorder : public QObject {
    Q_OBJECT
    public:
      Recorder(QObject *parent = nullptr);
      ~Recorder();

      // implicit title: of the observed action
      void begin_transaction(const QString &title,
          bool auto_commit = false);
      void commit();

      void connect_signals();

      // implicitly ends transaction
      void rewind();
      void forward();

      void set_depth(size_t number_of_transactions);

    public slots:
      void set_model(QAbstractItemModel *);

      void register_saved();
      void register_data_about_to_be_changed(
          const QModelIndex &topLeft, const QModelIndex &bottomRight,
          const QVector<int> & roles);
      void register_data_changed(
          const QModelIndex &topLeft, const QModelIndex &bottomRight,
          const QVector<int> & roles);
      void register_rows_about_to_be_removed(
          const QModelIndex &parent, int first, int last);
      void register_rows_removed(
          const QModelIndex &parent, int first, int last);
      void register_rows_about_to_be_inserted(
          const QModelIndex &parent, int first, int last);
      void register_rows_inserted(
          const QModelIndex &parent, int first, int last);
    signals:
      void transaction_begun(const QString &title);
      void transaction_committed();

      void model_changed(bool);
      void rewind_enabled(bool);
      void forward_enabled(bool);

      //XXX provide StringListModel with actions

    private:
      // call when executing an rewind/forward ...
      void pause();
      void record();
      void auto_commit();
      bool transaction_is_active() const;
      void update_revision(size_t revision);
      void push(std::unique_ptr<operation::Base> &&e);

      QAbstractItemModel *model_ {nullptr};
      bool paused_ {false};
      bool auto_commit_ {false};
      size_t save_point_ {0};
      size_t revision_ {0};
      size_t depth_ {std::numeric_limits<size_t>::max()};
      std::deque<std::unique_ptr<operation::Transaction> > rewind_stack_;
      // is cleared on each undo_stack.push_back
      // popping the rewind_stack -> top is moved to forward_stack
      std::deque<std::unique_ptr<operation::Transaction> > forward_stack_;

      std::unique_ptr<operation::Set_Data> set_data_op_;
      std::unique_ptr<operation::Remove_Rows> remove_rows_op_;



  };

}


#endif
