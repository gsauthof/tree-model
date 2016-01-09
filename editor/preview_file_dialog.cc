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
#include "preview_file_dialog.hh"

#include <QGridLayout>
#include <QSizePolicy>

namespace editor {

  Preview_File_Dialog::Preview_File_Dialog(QWidget *preview_widget,
      QWidget * parent, const QString &caption,
      const QString &directory,
      const QString &filter)
    :
      QFileDialog(parent, caption, directory, filter)
  {
    // By default, and if available, the system's file dialog is used.
    // Since we add a preview widget, we have to use the Qt one
    setOption(QFileDialog::DontUseNativeDialog);

    QGridLayout *grid_layout = dynamic_cast<QGridLayout*>(layout());
    if (grid_layout && preview_widget) {
      // default grid: 4 rows X 3 columns, where rows[1] is the main row
      // stretch = -1 -> stretches over all
      grid_layout->addWidget(preview_widget, 0 /*row*/, 3 /*col*/,
          -1 /*row stretch*/, -1 /*col stretch*/);
      connect(this, SIGNAL(currentChanged(const QString&)),
          preview_widget, SLOT(show_preview(const QString&)));
    }

    // overwrite default size policy which is
    // hor: Preferred, vert: Preferred
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
    // otherwise, the policy changes are not applied,
    // must be called after addWidget()
    adjustSize();
  }

}
