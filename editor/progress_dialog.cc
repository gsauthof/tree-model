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
#include "progress_dialog.hh"

#include <QTimer>
#include <QMessageBox>
#include <QDebug>
#include <QEventLoop>
#include <QKeyEvent>

namespace editor {

  Progress_Dialog::Progress_Dialog(const QString & labelText,
      const QString & cancelButtonText, QWidget *parent)
    :
      QProgressDialog(labelText, cancelButtonText, 0, 100, parent),
      timer_(new QTimer(this))
  {
    setMinimumDuration(1337);
    setWindowModality(Qt::WindowModal);

    connect(this, &QProgressDialog::canceled, timer_, &QTimer::stop);

    connect(timer_, &QTimer::timeout, this, &Progress_Dialog::update_progress);
    timer_->start(1000);
  }
  void Progress_Dialog::update_progress()
  {
    setValue(progress_value_ % 100);
    progress_value_ += 20;
  }

  void Progress_Dialog::wait()
  {
    QEventLoop e;
    // Use signal slot and not the new syntax because we ignore the argument
    connect(this, SIGNAL(finished(int)), &e, SLOT(quit()));
    connect(this, &Progress_Dialog::canceled, &e, &QEventLoop::quit);
    // We use exec to enter the event loop, such that
    // signals can be delivered (e.g. from the open command) - there are
    // 2 alternatives to do that:
    // a) call QEventLoop::exec() -> QProgressDialog is not displayed
    //                               for the first minimumDuration msecs
    // b) call QDialog::exec() -> QProgressDialog is immediately displayed
    e.exec();
  }

  void Progress_Dialog::prepare_accept()
  {
    timer_->stop();
    setValue(99);
    QTimer::singleShot(100, this, &QProgressDialog::accept);
  }

  void Progress_Dialog::finish()
  {
    prepare_accept();
  }

  void Progress_Dialog::display_error(const QString &msg)
  {
    reject();
    auto pw = parentWidget();
    // The lambda is used such that the progress dialog is directly closed.
    // Otherwise, it would pop up after the minimumDuration is over ...
    // We don't capture by reference to avoid accessing the already
    // freed message ...
    QTimer::singleShot(0, pw, [pw, msg](){
        QMessageBox::critical(pw, tr("Could not open file"), msg); });
  }

  void Progress_Dialog::keyPressEvent(QKeyEvent *event)
  {
    if (ignore_escape_ && event->key() == Qt::Key_Escape) {
      // ignore it
    } else {
      QDialog::keyPressEvent(event);
    }
  }
  void Progress_Dialog::set_ignore_escape(bool b)
  {
    ignore_escape_ = b;
  }

}
