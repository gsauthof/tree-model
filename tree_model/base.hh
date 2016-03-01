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
#ifndef TREE_MODEL_BASE_HH
#define TREE_MODEL_BASE_HH

#include <QObject>
#include <QVariant>
#include <QVector>

#include <memory>
#include <deque>

#include "index.hh"

class QMimeData;

namespace tree_model {

  class Base : public QObject
  {
    Q_OBJECT
    public:
      explicit Base(QObject *parent = nullptr);

      virtual void save(const QString &filename);

      virtual QVariant data(const Index &index,
          int role = Qt::DisplayRole) const = 0;
      virtual bool set_data(const Index &index, const QVariant &value,
          int role = Qt::EditRole);

      virtual QVariant header_data(size_t section, Qt::Orientation orientation,
          int role = Qt::DisplayRole) const;

      virtual unsigned column_count(const Index &parent = Index()) const = 0;

      virtual Qt::ItemFlags flags(const Index & index) const;

      virtual Index parent(const Index &index) const = 0;

      virtual bool has_children(const Index &parent = Index()) const = 0;

      virtual Index first_child(const Index &index = Index()) const = 0;
      virtual Index last_child(const Index &index = Index()) const = 0;

      virtual Index prev_sibling(const Index &index) const = 0;
      virtual Index next_sibling(const Index &index) const = 0;

      virtual Index attribute(const Index &index, unsigned column) const;

      virtual bool remove(const Index &index);
      
      virtual Index insert(const Index &index, int position);

      virtual QStringList mime_types() const;
      virtual std::unique_ptr<QMimeData> mime_data(
          const std::deque<Index> &indexes) const;
      virtual bool drop_mime_data(const QMimeData *data, Qt::DropAction action,
          const Index &index, int position);
      virtual Qt::DropActions supported_drop_actions() const;
      virtual bool can_drop_mime_data(const QMimeData *data,
          Qt::DropAction action, const Index &index, int position) const;

    signals:
      void data_about_to_be_changed(const tree_model::Index &index,
          const QVector<int> &roles = QVector<int>());
      void data_changed(const tree_model::Index &index,
          const QVector<int> &roles = QVector<int>());

      void index_about_to_be_removed(const tree_model::Index &index);
      void index_removed(const tree_model::Index &index);

      void index_about_to_be_inserted(const tree_model::Index &index,
          int position);
      void index_inserted(const tree_model::Index &index);

    public slots:

    protected:
      friend class Item_Adaptor;
      Index create_index(unsigned column, void *ptr = 0) const;

      void begin_remove_index(const Index &index);
      void end_remove_index(const Index &index);

      void begin_insert_index(const tree_model::Index &index, int position);
      void end_insert_index(const tree_model::Index &index);


        // match/search, persitent
  };

}

#endif
