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
#include "save.hh"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include <editor/progress_dialog.hh>

namespace editor {

  namespace gui_command {

    Save::Save(QWidget *parent)
      :
        Async_Save(static_cast<QObject*>(parent)),
        parent_widget_(parent)
    {
    }

    void Save::save()
    {
      save_as(filename());
    }
    void Save::save_as(const QString &filename)
    {
      Progress_Dialog progress_dialog(tr("Saving %1 ...").arg(filename),
          QString(), parent_widget_);
      progress_dialog.set_ignore_escape(true);

      connect(this, &Save::failed,
          &progress_dialog, &Progress_Dialog::display_error);
      connect(this, SIGNAL(saved(const QString&)),
          &progress_dialog, SLOT(finish()));

      Async_Save::save_as(filename);

      progress_dialog.wait();

      if (progress_dialog.result() == Progress_Dialog::Accepted
          && filename != this->filename())
        set_filename(filename);
    }
    QString Save::select_save_prime(const QString &caption)
    {
      QFileDialog file_dialog(parent_widget_, caption,
            QString(),
            tr("XML (*.xml)"));
      // for consistency with the open dialog ...
      file_dialog.setOption(QFileDialog::DontUseNativeDialog);
      file_dialog.setAcceptMode(QFileDialog::AcceptSave);
      QString filename;
      int r = file_dialog.exec();
      if (r) {
        auto filenames = file_dialog.selectedFiles();
        if (filenames.isEmpty()) // QFileDialog does not allow this
          QMessageBox::critical(parent_widget_, tr("Save error"),
              tr("No filename selected"));
        else if (filenames.size() > 1) // QFileDialog does not allow this
          QMessageBox::critical(parent_widget_, tr("Save error"),
              tr("More than one file selected"));
        else {
          filename = filenames.front();
          save_as(filename);
        }
      }
      return filename;
    }
    void Save::select_save()
    {
      QString filename(select_save_prime(tr("Select file to save to")));
      if (!filename.isEmpty() && filename != this->filename())
        set_filename(filename);
    }
    void Save::select_save_copy()
    {
      select_save_prime(tr("Select a file to save a copy to"));
    }

  }

}
