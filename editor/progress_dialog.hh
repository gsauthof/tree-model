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
#ifndef EDITOR_PROGRESS_DIALOG_HH
#define EDITOR_PROGRESS_DIALOG_HH

#include <QProgressDialog>

namespace editor {

  class Progress_Dialog : public QProgressDialog {
    Q_OBJECT
    public:
      explicit Progress_Dialog(const QString & labelText,
          const QString & cancelButtonText, QWidget *parent = nullptr);

      void wait();
      void set_ignore_escape(bool b);
    public slots:
      void finish();
      void display_error(const QString &msg);

    private slots:
      void update_progress();

    private:
      QTimer *timer_ {nullptr};
      unsigned progress_value_ {0};
      bool ignore_escape_ {false};

      void prepare_accept();

    protected:
      void keyPressEvent(QKeyEvent *event) override;
  };

}

#endif
