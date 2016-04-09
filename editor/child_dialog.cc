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
    ui->label->setEnabled(b);
  }

  QLineEdit &Child_Dialog::key_line()
  {
    return *ui->key_edit;
  }
  QLineEdit &Child_Dialog::value_line()
  {
    return *ui->value_edit;
  }

  void Child_Dialog::install_widget(QWidget *widget, unsigned column)
  {
    if (!widget)
      return;

    // should be re-parented again when replacing it in the
    // layout
    widget->setParent(this);
    if (auto e = dynamic_cast<QLineEdit*>(widget)) {
      e->setFrame(ui->key_edit->hasFrame());
      // the default Qt item delegate returns the sub-classed
      // QLineEdit, QExpandingLineEdit, that has automatic
      // resizing which is suboptimal for dialogs, thus, we
      // disable it
      // cf.
      // qtbase/src/widgets/itemviews/qitemeditorfactory.cpp
      disconnect(e, SIGNAL(textChanged(QString)),
          e, SLOT(resizeToContents()));
    }

    QWidget *from = column ? ui->value_edit : ui->key_edit;

    auto buddy = ui->label->buddy() == from ? ui->label : ui->label_2;
    auto a = ui->formLayout->replaceWidget(from, widget);
    if (!a)
      return;
    buddy->setBuddy(widget);
    buddy->setEnabled(widget->isEnabled());

    // deleting the widget would make the line edit getter return
    // a freed reference
    if (a) {
      a->widget()->hide();
      a->widget()->setParent(this);
    }
    // the destructor automatically hides the widget:
    // delete a->widget();
    delete a;

    if (!column)
      widget->setFocus();
  }

} // namespace editor
