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
#ifndef INSTANCE_HH
#define INSTANCE_HH

#include <QObject>

namespace editor {
  class Main_Window;
  class Gui_Controller;

  class Instance : public QObject {
    Q_OBJECT
    public:
      Instance(QObject *parent = nullptr);
      ~Instance();

    public slots:
      void open(const QString &filename);
      void show();
      void close();

    signals:
      void new_requested();
      void quit_requested();

    private:
      Main_Window *main_window_ {nullptr};
      Gui_Controller *gui_controller_ {nullptr};
  };

}

#endif
