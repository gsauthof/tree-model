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
#ifndef TREE_MODEL_XML_HH
#define TREE_MODEL_XML_HH

#include "base.hh"

#include <xxxml/xxxml.hh>


struct _xmlNode;
typedef struct _xmlNode xmlNode;

namespace tree_model {

  class XML : public Base
  {
    private:
      xxxml::doc::Ptr doc_;
    protected:
      Index create_index(unsigned column, const xmlNode *ptr = 0) const;
    public:
      XML(QObject *parent = nullptr);
      XML(xxxml::doc::Ptr &&doc, QObject *parent = nullptr);

      void save(const QString &filename) override;

      QVariant data(const Index &index, int role = Qt::DisplayRole) const override;
      bool set_data(const Index &index, const QVariant & value,
          int role = Qt::EditRole) override;

      QVariant header_data(size_t section, Qt::Orientation orientation,
          int role = Qt::DisplayRole) const override;

      unsigned column_count(const Index &parent = Index()) const override;

      Index parent(const Index &index) const override;

      bool has_children(const Index &index = Index()) const override;

      Index first_child(const Index &index = Index()) const override;
      Index last_child(const Index &index = Index()) const override;

      Index prev_sibling(const Index &index) const override;
      Index next_sibling(const Index &index) const override;

      Qt::ItemFlags flags(const Index &index) const override;

      bool remove(const Index &index) override;

      Index insert(const Index &index, int position) override;

      QStringList mime_types() const override;
      std::unique_ptr<QMimeData> mime_data(
          const std::deque<Index> &indexes) const override;
      bool drop_mime_data(const QMimeData *data, Qt::DropAction action,
          const Index &index, int position) override;

  };

}

#endif
