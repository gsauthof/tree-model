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
#ifndef EDITOR_PREVIEW_FILE_DIALOG_HH
#define EDITOR_PREVIEW_FILE_DIALOG_HH

#include <QFileDialog>

namespace editor {

  class Preview_File_Dialog : public QFileDialog {
    Q_OBJECT
    public:
      explicit Preview_File_Dialog(QWidget *preview_widget,
          QWidget *parent = nullptr, const QString &caption = QString(),
          const QString & directory = QString(),
          const QString & filter = QString());

  };

}

#endif
