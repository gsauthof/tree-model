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

#include <map>

using namespace std;

namespace editor {
  namespace gui_command {

    static QString xml_filter(QObject::tr("XML files (*.xml)"));

    static const map<QString, File_Type> filter_to_type_map_ = {
      { xml_filter                        ,   {File_Type::XML} },
      { QObject::tr("TAP files (CD* TD*)"), {File_Type::BER, File_Type::TAP} },
      { QObject::tr("RAP files (RC*)"),     {File_Type::BER, File_Type::RAP} },
      { QObject::tr("NRTRDE files (NR*)"),  {File_Type::BER, File_Type::NRT} },
      { QObject::tr("BER files (*.ber)"),   {File_Type::BER} },
      { QObject::tr("Any files (*)"),       {} }
    };
    static QStringList map_to_list(const map<QString, File_Type> &m)
    {
      QStringList r;
      for (auto &i : m)
        r << i.first;
      return r;
    }

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

      Preview_File_Dialog d(new Preview,
          parent_widget_, tr("Select file to open"));
      d.setFileMode(QFileDialog::ExistingFile);
      d.setNameFilters(map_to_list(filter_to_type_map_));
      d.selectNameFilter(xml_filter);

      connect(&d, &QFileDialog::fileSelected,
          [this, &d](const QString &filename) {
            Open::open_ft(filename,
                filter_to_type_map_.at(d.selectedNameFilter()));
          });

      d.exec();

    }


  } // gui_command
} // editor

