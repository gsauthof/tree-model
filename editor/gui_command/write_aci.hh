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
#ifndef EDITOR_GUI_COMMAND_WRITE_ACI_HH
#define EDITOR_GUI_COMMAND_WRITE_ACI_HH

#include <editor/command/write_aci.hh>

class QWidget;

namespace editor {

  namespace gui_command {

    class Write_ACI : public editor::command::Write_ACI
    {
      Q_OBJECT
      public:
        Write_ACI(QWidget *parent = nullptr);

      public slots:
        void write();

      signals:
        void was_canceled();

      private:
      protected:
        QWidget *parent_widget_ {nullptr};
    };

  } // gui_command

} // editor

#endif // EDITOR_GUI_COMMAND_WRITE_ACI_HH
