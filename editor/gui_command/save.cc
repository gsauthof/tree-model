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
      if (filename().isEmpty())
        select_save();
      else
        save_as(filename());
    }
    void Save::save_with_progress(const QString &filename, bool as_copy)
    {
      Progress_Dialog progress_dialog(tr("Saving %1 ...").arg(filename),
          QString(), parent_widget_);
      progress_dialog.set_ignore_escape(true);

      connect(this, &Save::failed,
          &progress_dialog, &Progress_Dialog::display_error);
      connect(this, SIGNAL(saved(const QString&)),
          &progress_dialog, SLOT(finish()));

      if (as_copy)
        Async_Save::save_copy_as(filename);
      else
        Async_Save::save_as(filename);

      progress_dialog.wait();
    }
    void Save::save_as(const QString &filename)
    {
      save_with_progress(filename, false);
    }
    void Save::save_copy_as(const QString &filename)
    {
      save_with_progress(filename, true);
    }
    QString Save::select_filename(const QString &caption)
    {
      QString filename;
      QFileDialog file_dialog(parent_widget_, caption,
            QString(),
            tr("XML (*.xml)"));
      // for consistency with the open dialog ...
      file_dialog.setOption(QFileDialog::DontUseNativeDialog);
      file_dialog.setAcceptMode(QFileDialog::AcceptSave);
      int r = file_dialog.exec();
      if (r) {
        auto filenames = file_dialog.selectedFiles();
        if (filenames.isEmpty()) // QFileDialog does not allow this
          QMessageBox::critical(parent_widget_, tr("Save error"),
              tr("No filename selected"));
        else if (filenames.size() > 1) // QFileDialog does not allow this
          QMessageBox::critical(parent_widget_, tr("Save error"),
              tr("More than one file selected"));
        else
          filename = filenames.front();
      }
      return filename;
    }
    void Save::select_save()
    {
      QString filename(select_filename(tr("Select file to save to")));
      save_as(filename);
    }
    void Save::select_save_copy()
    {
      QString filename(select_filename(tr("Select a file to save a copy to")));
      save_copy_as(filename);
    }

  }

}
