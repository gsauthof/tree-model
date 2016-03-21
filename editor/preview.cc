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
#include "preview.hh"
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QFile>

#include <boost/filesystem.hpp>

static bool looks_like_text(const QByteArray &array)
{
  return std::all_of(array.begin(), array.end(), [](auto b){
      return (b > 31 && b != 127) || b == '\t' || b == '\n' || b == '\r';
      });
}

namespace editor {

  Preview::Preview(QWidget *parent)
    :
      QWidget(parent)
  {
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
          QSizePolicy::Expanding));
    auto layout = new QVBoxLayout;

    //auto label = new QLabel(tr("Preview:"));

    text_edit_ = new QTextEdit;
    auto font_metrics = text_edit_->fontMetrics();
    text_edit_->setMinimumWidth(font_metrics.averageCharWidth()*40);
    text_edit_->setReadOnly(true);
    text_edit_->setPlainText(tr("Preview"));
    text_edit_->setEnabled(false);

    //layout->addWidget(label);
    layout->addWidget(text_edit_);
    setLayout(layout);
  }

  const QTextEdit &Preview::text_edit() const
  {
    return *text_edit_;
  }

  void Preview::set_delegate(std::function<QString(const QString &filename)> fn)
  {
    delegate_ = fn;
  }

  void Preview::show_preview(const QString &filename)
  {
    text_edit_->clear();
    text_edit_->setEnabled(false);

    if (!boost::filesystem::is_regular_file(filename.toStdString())) {
      text_edit_->clear();
      return;
    }

    if (delegate_) {
      auto text = delegate_(filename);
      if (!text.isEmpty()) {
        text_edit_->setEnabled(true);
        text_edit_->setPlainText(std::move(text));
        //text_edit_->setHtml();
        return;
      }
    }

    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      auto array = file.read(4 * 1024);
      if (looks_like_text(array)) {
        text_edit_->setEnabled(true);
        text_edit_->setPlainText(QString(array));
      }
    }
  }

}
