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
#include "open.hh"

#include <editor/progress_dialog.hh>

#include <tree_model/base.hh>

#include <QWidget>
#include <QDebug>

namespace editor {
namespace gui_command {

  Open::Open(QWidget *parent)
    :
      Async_Open(static_cast<QObject*>(parent)),
      parent_widget_(parent)
  {
  }
  void Open::open(const QString &filename)
  {
    Progress_Dialog progress_dialog(tr("Opening %1 ...").arg(filename),
        tr("&Cancel"), parent_widget_);
    connect(&progress_dialog, &Progress_Dialog::canceled,
        this, &Open::ignore_result);

    connect(this, &Open::failed,
        &progress_dialog, &Progress_Dialog::display_error);
    connect(this, SIGNAL(file_opened(const QString&)),
        &progress_dialog, SLOT(finish()));

    Async_Open::open(filename);

    progress_dialog.wait();
  }
}
}
