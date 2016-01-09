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
#ifndef EDITOR_RECENT_MENU_HH
#define EDITOR_RECENT_MENU_HH

#include <QMenu>

namespace editor {

  class Recent_Menu : public QMenu
  {
    Q_OBJECT
    public:
      Recent_Menu(QWidget *parent = nullptr);
      Recent_Menu(const QString &title,
          QWidget *parent = nullptr);

      void set_size(size_t n);

    signals:
      void chosen(const QString &filename);

    public slots:
      void register_file_open(const QString &filename);

    private slots:
      void register_trigger(QAction *action);
    private:
      size_t size_ {10};

      QAction *find_action(const QString &filename, const
          QList<QAction*> &l);
      void renumerate_actions(const QList<QAction*> &l, size_t i = 1);
      QAction *create_action(size_t i, const QString &filename);

      void restore();
      void store();
  };

}

#endif
