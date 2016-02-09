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
#include "slider_bar.hh"
#include "ui_slider_bar.h"

#include <QIntValidator>
#include <QShortcut>

#include "absolute_slider_style.hh"

namespace editor {

  Slider_Bar::Slider_Bar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Slider_Bar)
  {
    ui->setupUi(this);

    auto font_metrics = fontMetrics();
    ui->clear_button->setMaximumWidth(font_metrics.averageCharWidth()*3);
    ui->ok_button->setMaximumWidth(font_metrics.averageCharWidth()*3);
    ui->rank_edit->setMaximumWidth(font_metrics.averageCharWidth()*8);

    ui->rank_edit->setValidator(new QIntValidator(this));
    auto edit_shortcut = new QShortcut(
        QKeySequence(tr("Ctrl+G", "Goto rank edit")), this);
    connect(edit_shortcut, &QShortcut::activated,
        [this]() { ui->rank_edit->setFocus(Qt::ShortcutFocusReason); });

    ui->rank_slider->setMinimum(1);
    ui->rank_slider->setTickPosition(QSlider::TicksBelow);
    // this yields a memory leak report with leaksanitizer,
    // but setting a parent or explicitly deleting it
    // in the destructor (after the ui is deleted) yields a
    // segfault ...
    // ... apparently the base style is re-parented by proxy-style
    // but uncoditionally freed at application exit ...
    static QProxyStyle *style = nullptr;
    if (!style)
      style = new Absolute_Slider_Style(ui->rank_slider->style());
    ui->rank_slider->setStyle(style);

    setup_slider_machine();
  }

  Slider_Bar::~Slider_Bar()
  {
    delete ui;
  }

  void Slider_Bar::setup_slider_machine()
  {
    // same as valueChanged when tracking is enabled (the default),
    // except when slider value is changed without moving ...
    // (e.g. on a direct click)
    //connect(ui->rank_slider, &QSlider::sliderMoved,
    connect(ui->rank_slider, &QSlider::valueChanged,
        [this](int value) {
          ui->rank_edit->setText(QString::number(value));
        });
    connect(ui->rank_slider, &QSlider::sliderReleased,
        [this]() {
          auto value = ui->rank_slider->value();
          emit jump_requested(value);
        });

    connect(ui->rank_edit, &QLineEdit::editingFinished,
        this, &Slider_Bar::jump_to_edit_line_rank );
    connect(ui->clear_button, &QPushButton::clicked,
        [this]() { ui->rank_edit->clear(); });
    connect(ui->ok_button, &QPushButton::clicked,
        this, &Slider_Bar::jump_to_edit_line_rank );
  }

  void Slider_Bar::init(int pos, int n)
  {
    ui->max_rank_label->setText(QString("/%1").arg(n));
    ui->rank_slider->setMaximum(n);
    if (n > 50)
      ui->rank_slider->setTickInterval(n/50);
    ui->rank_slider->setValue(pos);
    ui->rank_edit->setText(QString::number(pos));
  }

  void Slider_Bar::jump_to_edit_line_rank()
  {
    bool ok = false;
    int value = ui->rank_edit->text().toInt(&ok);
    if (ok && value > 0) {
      ui->rank_slider->setValue(value);
      emit jump_requested(value);
    }
  }

} // namespace editor
