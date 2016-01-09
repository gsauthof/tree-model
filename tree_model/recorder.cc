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
#include "recorder.hh"

#include "operation/transaction.hh"
#include "operation/set_data.hh"
#include "operation/insert_rows.hh"
#include "operation/remove_rows.hh"

#include <QAbstractItemModel>
#include <QDebug>
#include <stdexcept>


using namespace std;

namespace tree_model {

  Recorder::Recorder(QObject *parent)
    :
      QObject(parent)
  {
  }
  Recorder::~Recorder() =default;

  void Recorder::connect_signals()
  {
    if (!model_)
      throw logic_error("No model set");
    connect(model_, SIGNAL(dataAboutToBeChanged(const QModelIndex&,
            const QModelIndex&, const QVector<int> &)),
        this, SLOT(register_data_about_to_be_changed(const QModelIndex&,
            const QModelIndex&, const QVector<int> &
            )));
    connect(model_, &QAbstractItemModel::dataChanged,
        this, &Recorder::register_data_changed);
    connect(model_, &QAbstractItemModel::rowsAboutToBeInserted,
        this, &Recorder::register_rows_about_to_be_inserted);
    connect(model_, &QAbstractItemModel::rowsInserted,
        this, &Recorder::register_rows_inserted);
    connect(model_, &QAbstractItemModel::rowsAboutToBeRemoved,
        this, &Recorder::register_rows_about_to_be_removed);
    connect(model_, &QAbstractItemModel::rowsRemoved,
        this, &Recorder::register_rows_removed);
  }

  bool Recorder::transaction_is_active() const
  {
    return  !rewind_stack_.empty()
        && rewind_stack_.back()->state() == operation::Base::INITIALIZED;
  }

  void Recorder::set_depth(size_t number_of_transactions)
  {
    depth_ = number_of_transactions;
  }

  void Recorder::begin_transaction(const QString &title, bool auto_commit)
  {
    if (transaction_is_active())
      return;
    auto_commit_ = auto_commit;

    rewind_stack_.push_back(make_unique<operation::Transaction>(title));
    if (rewind_stack_.size() > depth_)
      rewind_stack_.erase(rewind_stack_.begin(),
          rewind_stack_.begin() + rewind_stack_.size() - depth_);
    emit transaction_begun(title);

    if (!forward_stack_.empty()) {
      forward_stack_.clear();
      emit forward_enabled(false);
    }
  }

  void Recorder::auto_commit()
  {
    if (!auto_commit_)
      return;
    auto_commit_ = false;
    commit();
  }

  void Recorder::commit()
  {
    if (rewind_stack_.empty())
      return;
    if (rewind_stack_.back()->state() == operation::Base::INITIALIZED) {
      if (rewind_stack_.back()->empty())
        rewind_stack_.pop_back();
      else
        rewind_stack_.back()->commit();
      emit transaction_committed();
    }
  }

  void Recorder::set_model(QAbstractItemModel *model)
  {
    model_ = model;
    connect_signals();
    emit model_changed(false);
    emit rewind_enabled(false);
    emit forward_enabled(false);
    rewind_stack_.clear();
    forward_stack_.clear();
    save_point_ = 0;
    revision_ = 0;
  }
  void Recorder::register_saved()
  {
    emit model_changed(false);
    save_point_ =  revision_;
  }
  void Recorder::register_data_about_to_be_changed(
      const QModelIndex &top_left, const QModelIndex &bottom_right,
      const QVector<int> & roles)
  {
    if (paused_)
      return;
    if (!(roles.empty() || roles.contains(Qt::EditRole)))
      return;

    begin_transaction(tr("set data"), true);
    set_data_op_ = make_unique<operation::Set_Data>(top_left, bottom_right,
        *model_);
  }
  void Recorder::register_data_changed(
      const QModelIndex &top_left, const QModelIndex &bottom_right,
      const QVector<int> & roles)
  {
    if (paused_)
      return;
    if (!(roles.empty() || roles.contains(Qt::EditRole)))
      return;
    set_data_op_->update(top_left, bottom_right, *model_);
    if (rewind_stack_.empty())
      return;
    push(std::move(set_data_op_));
    auto_commit();
  }
  void Recorder::push(std::unique_ptr<operation::Base> &&e)
  {
    if (revision_ == save_point_)
      emit model_changed(true);
    rewind_stack_.back()->push(std::move(e));
    ++revision_;
    if (rewind_stack_.size() == 1 && rewind_stack_.back()->size() == 1)
      emit rewind_enabled(true);
  }

  void Recorder::register_rows_about_to_be_inserted(
      const QModelIndex &parent, int first, int last)
  {
  }
  void Recorder::register_rows_inserted(
      const QModelIndex &parent, int first, int last)
  {
    if (paused_)
      return;
    begin_transaction(tr("insert rows"), true);
    push(make_unique<operation::Insert_Rows>(parent, first, last));
    auto_commit();
  }

  void Recorder::register_rows_about_to_be_removed(
      const QModelIndex &parent, int first, int last)
  {
    if (paused_)
      return;
    begin_transaction(tr("remove rows"), true);
    remove_rows_op_ = make_unique<operation::Remove_Rows>(parent, first, last,
        *model_);
  }
  void Recorder::register_rows_removed(
      const QModelIndex &parent, int first, int last)
  {
    if (paused_)
      return;
    push(std::move(remove_rows_op_));
    auto_commit();
  }

  void Recorder::rewind()
  {
    if (rewind_stack_.empty())
      return;
    update_revision(revision_ - 1);
    pause();
    auto op = std::move(rewind_stack_.back());
    if (op->state() == operation::Base::INITIALIZED)
      op->commit();
    rewind_stack_.pop_back();
    if (rewind_stack_.empty())
      emit rewind_enabled(false);
    op->rewind(*model_);
    forward_stack_.push_back(std::move(op));
    if (forward_stack_.size() == 1)
      emit forward_enabled(true);
    record();
  }
  void Recorder::forward()
  {
    if (forward_stack_.empty())
      return;
    update_revision(revision_ + 1);
    pause();
    auto op = std::move(forward_stack_.back());
    forward_stack_.pop_back();
    if (forward_stack_.empty())
      emit forward_enabled(false);
    op->forward(*model_);
    rewind_stack_.push_back(std::move(op));
    if (rewind_stack_.size() > depth_)
      rewind_stack_.erase(rewind_stack_.begin(),
          rewind_stack_.begin() + rewind_stack_.size() - depth_);
    if (rewind_stack_.size() == 1)
      emit rewind_enabled(true);
    record();
  }

  void Recorder::update_revision(size_t revision)
  {
    if (revision_ == revision)
      return;
    if (revision_ == save_point_)
      emit model_changed(true);
    revision_ = revision;
    if (revision_ == save_point_)
      emit model_changed(false);
  }

  void Recorder::pause()
  {
    paused_ = true;
  }
  void Recorder::record()
  {
    paused_ = false;
  }

}
