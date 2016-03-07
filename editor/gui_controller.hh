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
#ifndef EDITOR_GUI_CONTROLLER_HH
#define EDITOR_GUI_CONTROLLER_HH

#include <QAbstractItemModel>
#include <QString>
#include <QList>
#include <QUrl>

class QItemSelectionModel;

#include "controller.hh"


namespace editor {

  class Subtree_Window;
  namespace gui_command {
    class Open;
    class Select_Open;
    class Save;
    class Edit;
    class Add;
    class Clipboard_Copy;
    class Clipboard_Cut;
    class Clipboard_Paste;
    class Display_Subtree;
  }

  class Gui_Controller : public Controller {
    Q_OBJECT

    public:
      explicit Gui_Controller(QWidget *parent = nullptr);

    public slots:
      void open(const QString &filename);
      void open_urls(const QList<QUrl> &urls);
      void select_open();
      void save();
      void select_save();
      void select_save_copy();
      void clipboard_cut(const QModelIndexList &is);
      void clipboard_copy(const QModelIndexList &is);
      void clipboard_paste(const QModelIndexList &is);
      void clipboard_paste_as_child(const QModelIndexList &is);
      void display_subtree();
      void add_child(const QModelIndex &i);
      void add_sibling(const QModelIndex &i);
      void edit(const QModelIndex &i);

    private:
      QWidget                           *parent_widget_        {nullptr};

      gui_command::Open                 *open_                 {nullptr};
      gui_command::Select_Open          *select_open_          {nullptr};
      gui_command::Save                 *save_                 {nullptr};
      gui_command::Edit                 *edit_                 {nullptr};
      gui_command::Add                  *add_                  {nullptr};
      gui_command::Clipboard_Copy       *clipboard_copy_       {nullptr};
      gui_command::Clipboard_Cut        *clipboard_cut_        {nullptr};
      gui_command::Clipboard_Paste      *clipboard_paste_      {nullptr};
      gui_command::Display_Subtree      *display_subtree_      {nullptr};

    signals:
      void selection_model_changed(const QItemSelectionModel *smodel);
      void subtree_window_created(editor::Subtree_Window *w);;
      void open_more_urls_requested(const QList<QUrl> &urls);


  };

}

#endif
