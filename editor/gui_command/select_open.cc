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
#include "select_open.hh"

#include <QFileDialog>
#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QSizePolicy>

#include "open.hh"

#include <editor/preview_file_dialog.hh>
#include <editor/preview.hh>
#include <tree_model/base.hh>
#include <editor/file_type.hh>

#include <editor/command/preview.hh>


using namespace std;

namespace editor {
  namespace gui_command {

    Select_Open::Select_Open(QWidget *parent)
      :
        Open(parent)
    {
    }
    void Select_Open::open()
    {
      // The preview widget should have a reasonable minimum size set.
      // Or a size policy like Mimimum or MinimumExpanding (horizontally), e.g.:
      // w.setSizePolicy(QSizePolicy(QSizePolicy::Minimum,
      //      QSizePolicy::Expanding));
      // Otherwise, it is not optimally displayed in the dialog.

      // assuming that it is reparented by addWidget()
      auto preview = new Preview;
      preview->set_delegate(editor::command::preview);
      Preview_File_Dialog d(preview,
          parent_widget_, tr("Select file to open"));
      d.setFileMode(QFileDialog::ExistingFile);
      d.setNameFilters(all_name_filters());
      d.selectNameFilter(default_open_name_filter());

      connect(&d, &QFileDialog::fileSelected,
          [this, &d](const QString &filename) {
            Open::open_ft(filename,
               File_Type::construct_from_name_filter(d.selectedNameFilter()));
          });

      d.exec();

    }


  } // gui_command
} // editor

