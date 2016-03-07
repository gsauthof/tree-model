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
#include <editor/file_type.hh>

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
    void Open::open_prime(const QString &filename, std::function<void()> f)
    {
      Progress_Dialog progress_dialog(tr("Opening %1 ...").arg(filename),
          tr("&Cancel"), parent_widget_);
      connect(&progress_dialog, &Progress_Dialog::canceled,
          this, &Open::ignore_result);

      connect(this, &Open::failed,
          &progress_dialog, &Progress_Dialog::display_error);
      connect(this, SIGNAL(file_opened(const QString&)),
          &progress_dialog, SLOT(finish()));

      f();

      progress_dialog.wait();
    }
    void Open::open(const QString &filename)
    {
      open_prime(filename, [this, &filename](){ Async_Open::open(filename); });
    }
    void Open::open_ft(const QString &filename, const File_Type &ft)
    {
      open_prime(filename, [this, &filename, &ft](){
          Async_Open::open_ft(filename, ft); });
    }

    void Open::set_modified(bool b)
    {
      modified_ = b;
    }

    void Open::open_urls(const QList<QUrl> &urls_P)
    {
      if (urls_P.empty())
        return;
      auto urls = urls_P;
      if (!modified_) {
        auto u = urls.takeFirst();
        open(u.toLocalFile());
      }
      if (!urls.empty())
        emit open_more_urls_requested(urls);
    }

  } // gui_command
} // editor
