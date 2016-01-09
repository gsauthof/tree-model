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
#include "recent_menu.hh"

#include <boost/filesystem.hpp>

#include <QAction>
#include <QSettings>
#include <QDebug>

using namespace std;

namespace editor {

  namespace key {
    const char recent_files[] = "recent_files";
    const char filename[]     = "filename";
  }

  Recent_Menu::Recent_Menu(QWidget *parent)
    :
      Recent_Menu(tr("Open &Recent"), parent)
  {
  }

  Recent_Menu::Recent_Menu(const QString &title, QWidget *parent)
    :
      QMenu(title, parent)
  {
    connect(this, &Recent_Menu::triggered,
        this, &Recent_Menu::register_trigger);
    restore();
  }

  void Recent_Menu::restore()
  {
    QSettings s;
    int n = s.beginReadArray(key::recent_files);
    for (int i = 0; i < n; ++i) {
      s.setArrayIndex(i);
      auto a = create_action(i+1, s.value(key::filename).toString());
      addAction(a);
    }
    s.endArray();
  }

  void Recent_Menu::store()
  {
    QSettings s;
    auto l = actions();
    s.beginWriteArray(key::recent_files, l.size());
    int i = 0;
    for (auto a : l) {
      s.setArrayIndex(i);
      s.setValue(key::filename, a->data());
      ++i;
    }
    s.endArray();
  }

  void Recent_Menu::set_size(size_t n)
  {
    size_ = n;
  }

  QAction *Recent_Menu::find_action(const QString &filename, const
      QList<QAction*> &l)
  {
    QVariant f(filename);
    for (auto a : l)
      if (a->data() == f)
        return a;
    return nullptr;
  }

  QAction *Recent_Menu::create_action(size_t i, const QString &filename)
  {
    auto a = new QAction(QString("&%1. %2").arg(i).arg(boost::filesystem::path(
            filename.toUtf8().data()).filename().generic_string().c_str()),
        this);
    a->setData(filename);
    return a;
  }

  void Recent_Menu::register_file_open(const QString &filename)
  {
    auto l = actions();
    QAction *q = find_action(filename, l);
    if (q) {
      removeAction(q);
      insertAction(l.front(), q);
    } else {
      auto front = l.empty() ? nullptr : l.front();
      auto a = create_action(1, filename);
      insertAction(front, a);
      if (l.size() + 1 > int(size_)) {
        size_t n = l.size() + 1 - size_;
        for (size_t i = 0; i<n; ++i)
          removeAction(l.takeLast());
      }
    }
    renumerate_actions(l, 2);
    store();
  }

  void Recent_Menu::renumerate_actions(const QList<QAction*> &l, size_t i)
  {
    for (auto a : l) {
      auto filename = a->data().toString();
      a->setText(QString("&%1. %2").arg(i).arg(boost::filesystem::path(
              filename.toUtf8().data()).filename().generic_string().c_str()));
      ++i;
    }
  }

  void Recent_Menu::register_trigger(QAction *action)
  {
    QString s(action->data().toString());
    if (!s.isEmpty())
      emit chosen(s);
  }


}
