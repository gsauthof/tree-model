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
#include "xml.hh"

#include <xxxml/xxxml.hh>
#include <xxxml/util.hh>

#include <QMimeData>
#include <QDebug>

#include <string>


namespace tree_model {

  void XML::save(const QString &filename)
  {
    xxxml::save_format_file_enc(filename.toUtf8().data(), doc_);
  }

  Index XML::create_index(unsigned column, const xmlNode *ptr) const
  {
    return Base::create_index(column, const_cast<xmlNode*>(ptr));
  }

  XML::XML(xxxml::doc::Ptr &&doc, QObject *parent)
    :
      doc_(std::move(doc))
  {
  }
  XML::XML(QObject *parent)
    :
      Base(parent),
      doc_(xxxml::read_memory(""))
  {
  }


  QVariant XML::data(const Index &index, int role) const
  {
    if (index.is_valid()) {
      const xmlNode *node = static_cast<const xmlNode*>(index.internal_pointer());
      switch (index.column()) {
        case 0:
          switch (role) {
            case Qt::EditRole:
            case Qt::DisplayRole:
              return QVariant(QString(xxxml::name(node)));
              break;
            default:
              break;
          }
          break;
        case 1:
          switch (role) {
            case Qt::EditRole:
            case Qt::DisplayRole:
              if (!has_children(attribute(index, 0)) && node->children)
                return QVariant(QString(xxxml::content(node->children)));
              break;
            default:
              break;
          }
          break;
        default:
          break;
      }
    } else {
      return QVariant();
    }
    return QVariant();
  }

  bool XML::set_data(const Index &index, const QVariant &value, int role)
  {
    if (!index.is_valid())
      return false;
    bool r = false;
    xmlNode *node = static_cast<xmlNode*>(index.internal_pointer());
    switch (role) {
      case Qt::EditRole:
        switch (index.column()) {
          case 0:
            emit data_about_to_be_changed(index);
            xxxml::node_set_name(node, value.toString().toUtf8().data());
            r = true;
            break;
          case 1:
            if (!has_children(attribute(index, 0))) {
              emit data_about_to_be_changed(index);
              xxxml::util::set_content(node, value.toString().toUtf8().data());
              r = true;
            }
            break;
        }
        break;
    }
    if (r) {
      emit data_changed(index);
    }
    return r;
  }

  unsigned XML::column_count(const Index &parent) const
  {
    return 2;
  }

  Index XML::parent(const Index &index) const
  {
    // we don't require index.column() == 0, this
    // matches the parent semantics of QAbstractItemModel
    if (index.is_valid()) {
      const xmlNode *node = static_cast<const xmlNode*>(index.internal_pointer());
      if (node->parent && node->parent->type == XML_ELEMENT_NODE)
        return create_index(0, node->parent);
    }
    return Index();
  }

  bool XML::has_children(const Index &index) const
  {
    if (index.column())
      return false;
    if (index.is_valid()) {
      const xmlNode *node = static_cast<const xmlNode*>(index.internal_pointer());
      const xmlNode *child = xxxml::first_element_child(node);
      return child;
    } else {
      return xxxml::util::has_root(doc_);
    }
  }

  Index XML::first_child(const Index &index) const
  {
    if (index.column())
      return Index();
    if (index.is_valid()) {
      const xmlNode *node = static_cast<const xmlNode*>(index.internal_pointer());
      const xmlNode *child = xxxml::first_element_child(node);
      return create_index(0, child);
    } else {
      if (has_children(index))
        return create_index(0, xxxml::doc::get_root_element(doc_));
      else
        return Index();
    }
  }

  Index XML::last_child(const Index &index) const
  {
    if (index.column())
      return Index();
    if (index.is_valid()) {
      const xmlNode *node = static_cast<const xmlNode*>(index.internal_pointer());
      const xmlNode *child = xxxml::last_element_child(node);
      return create_index(0, child);
    } else {
      if (has_children(index))
        return create_index(0, xxxml::doc::get_root_element(doc_));
      else
        return Index();
    }
  }

  Index XML::prev_sibling(const Index &index) const
  {
    if (index.is_valid() && !index.column()) {
      const xmlNode *node = static_cast<const xmlNode*>(index.internal_pointer());
      const xmlNode *e = xxxml::previous_element_sibling(node);
      return create_index(0, e);
    } else {
      return Index();
    }
  }

  Index XML::next_sibling(const Index &index) const
  {
    if (index.is_valid() && !index.column()) {
      const xmlNode *node = static_cast<const xmlNode*>(index.internal_pointer());
      const xmlNode *e = xxxml::next_element_sibling(node);
      return create_index(0, e);
    } else {
      return Index();
    }
  }

  Qt::ItemFlags XML::flags(const Index &index) const
  {
    if (index.is_valid()) {
      Qt::ItemFlags r = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
      switch (index.column()) {
        case 0:
          r |= Qt::ItemIsEditable;
          break;
        case 1:
          if (!has_children(attribute(index, 0)))
            r |= Qt::ItemIsEditable;
          break;
      }
      return r;
    } else
      return 0;
  }

  bool XML::remove(const Index &index)
  {
    if (index.is_valid()) {
      begin_remove_index(index);
      xmlNode *node = static_cast<xmlNode*>(index.internal_pointer());
      xmlNode *parent = node->parent;
      xxxml::unlink_node(node);
      if (parent) {
        // otherwise, whitespace is left over ...
        if (!xxxml::first_element_child(parent))
          xxxml::util::set_content(parent, std::string());
      }
      end_remove_index(index);
      return true;
    } else {
      return false;
    }
  }

  Index XML::insert(const Index &index, int position)
  {
    if (!index.is_valid() && (position == 2 || position == -2))
      return Index();
    if (index.is_valid() && (position == 1 || position == -1)
        && !has_children(index) && data(attribute(index, 1)).isValid())
      return Index();
    try {
      begin_insert_index(index, position);
      xmlNode *new_node = xxxml::new_doc_node(doc_, "NEW NODE");
      if (index.is_valid()) {
        xmlNode *node = static_cast<xmlNode*>(index.internal_pointer());
        xxxml::util::insert(doc_, node, new_node, position);
      } else {
        xxxml::doc::set_root_element(doc_, new_node);
      }
      auto new_index = create_index(0, new_node);
      end_insert_index(new_index);
      return new_index;
    } catch (const std::exception &e) {
      return Index();
    }
  }

  QStringList XML::mime_types() const
  {
    QStringList r;
    r << "text/xml";
    return r;
  }
  std::unique_ptr<QMimeData> XML::mime_data(
      const std::deque<Index> &indexes) const
  {
    if (indexes.empty())
      return std::unique_ptr<QMimeData>();
    auto r = std::make_unique<QMimeData>();
    // XXX support index lists > 1?
    Index i(indexes.front());
    if (i.is_valid()) {
      const xmlNode *node = static_cast<const xmlNode*>(i.internal_pointer());
      auto x = xxxml::util::dump(doc_, node);
      r->setData("text/xml", QByteArray(x.first.first,
            x.first.second - x.first.first));
    } else {
      auto p = xxxml::doc::dump_format_memory(doc_);
      r->setData("text/xml", QByteArray(p.first.get(), p.second));
    }
    return r;
  }
  bool XML::drop_mime_data(const QMimeData *data, Qt::DropAction action,
      const Index &i, int position)
  {
    if (!data)
      return false;
    if (action == Qt::CopyAction) {
      xmlNode *node = static_cast<xmlNode*>(i.internal_pointer());
      QByteArray a(data->data("text/xml"));
      try {
        auto new_node = xxxml::util::create_node(doc_,
            a.data(), a.data() + a.size());
        begin_insert_index(i, position);
        auto x = new_node.release();
        xxxml::util::insert(doc_, node, x, position);
        end_insert_index(create_index(0, x));
      } catch (const xxxml::Parse_Error &e) {
        return false;
      }
      return true;
    } else {
      return false;
    }
  }

  QVariant XML::header_data(size_t section, Qt::Orientation orientation,
      int role) const
  {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
      if (section == 0)
        return tr("Key");
      else
        return tr("Value");
    } else
      return QVariant();
  }

}

