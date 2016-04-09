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
#include "child_dialog.hh"
#include "ui_child_dialog.h"

namespace editor {

  Child_Dialog::Child_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Child_Dialog)
  {
    ui->setupUi(this);
  }

  Child_Dialog::~Child_Dialog()
  {
    delete ui;
  }

  void Child_Dialog::set_data(const QString &key, const QString &value)
  {
    ui->key_edit->setText(key);
    ui->value_edit->setText(value);
  }
  QString Child_Dialog::key() const
  {
    return ui->key_edit->text();
  }
  QString Child_Dialog::value() const
  {
    return ui->value_edit->text();
  }
  void Child_Dialog::enable_value(bool b)
  {
    ui->value_edit->setEnabled(b);
  }

  QLineEdit &Child_Dialog::key_line()
  {
    return *ui->key_edit;
  }
  QLineEdit &Child_Dialog::value_line()
  {
    return *ui->value_edit;
  }

} // namespace editor
