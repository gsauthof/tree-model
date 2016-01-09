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
#ifndef MAIN_WINDOW_HH
#define MAIN_WINDOW_HH

#include <QMainWindow>


namespace editor {
  class Gui_Controller;

  namespace Ui {
    class Main_Window;
  }

  class Main_Window : public QMainWindow
  {
    Q_OBJECT

    public:
      explicit Main_Window(QWidget *parent = nullptr);
      ~Main_Window();

    public slots:
      void open(const QString &filename);
    private slots:
      void display_status(const QString &msg);
      void update_window_title(const QString &filename);
    private:
      Ui::Main_Window *ui;
      Gui_Controller *controller_ {nullptr};

      void setup_shortcuts();
  };

}

#endif // MAIN_WINDOW_HH
