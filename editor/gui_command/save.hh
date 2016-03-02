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
#ifndef EDITOR_GUI_COMMAND_SAVE_HH
#define EDITOR_GUI_COMMAND_SAVE_HH

#include <editor/command/async_save.hh>

namespace editor {
  namespace gui_command {

    class Save : public editor::command::Async_Save {
      Q_OBJECT
      public:
        explicit Save(QWidget *parent = nullptr);

      public slots:
        void save();
        void save_as(const QString &filename);
        void save_copy_as(const QString &filename);
        void select_save();
        void select_save_copy();

      protected:
        QWidget *parent_widget_ {nullptr};

        QString select_filename(const QString &caption);
        void save_with_progress(const QString &filename, bool as_copy);

    };

  }
}


#endif
