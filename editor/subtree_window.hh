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

#ifndef EDITOR_SUBTREE_WINDOW_HH
#define EDITOR_SUBTREE_WINDOW_HH

#include <QDialog>

namespace editor {

  class Tree_Widget;

  namespace Ui {
    class Subtree_Window;
  }

  class Subtree_Window : public QDialog
  {
    Q_OBJECT

    public:
      explicit Subtree_Window(QWidget *parent = 0);
      ~Subtree_Window();

      Tree_Widget &tree_widget();
    private:
      Ui::Subtree_Window *ui;
  };


} // namespace editor
#endif // EDITOR_SUBTREE_WINDOW_HH
