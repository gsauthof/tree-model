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

#include "controller.hh"


namespace editor {

  namespace gui_command {
    class Open;
    class Select_Open;
    class Save;
    class Display_Tree_Context;
    class Edit;
    class Add;
  }

  class Gui_Controller : public Controller {
    Q_OBJECT

    public:
      explicit Gui_Controller(QWidget *parent = nullptr);

    public slots:
      void open(const QString &filename);
      void select_open();
      void save();
      void select_save();
      void select_save_copy();
      void display_tree_context(const QPoint &global_pos,
            const QModelIndex &context_index,
            const QModelIndexList &selected_indexes);

    private:
      QWidget                           *parent_widget_        {nullptr};

      gui_command::Open                 *open_                 {nullptr};
      gui_command::Select_Open          *select_open_          {nullptr};
      gui_command::Save                 *save_                 {nullptr};
      gui_command::Display_Tree_Context *display_tree_context_ {nullptr};
      gui_command::Edit                 *edit_                 {nullptr};
      gui_command::Add                  *add_                  {nullptr};


  };

}

#endif
