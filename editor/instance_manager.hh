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
#ifndef INSTANCE_MANAGER_HH
#define INSTANCE_MANAGER_HH

#include <QObject>
#include <QList>
#include <QUrl>

#include <deque>

namespace editor {

  class Instance;

  class Instance_Manager : public QObject {
    Q_OBJECT

    public:
      Instance_Manager(QObject *parent = nullptr);

      Instance *create_instance();
    public slots:
      void do_create_instance();
      void quit();

      void open_urls_in_new_instances(const QList<QUrl> &urls);

    signals:
      void first_open_requested(const QString &filename);

    private:
      std::deque<Instance*> instances_;



  };

}

#endif

