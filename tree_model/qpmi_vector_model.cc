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

#include "qpmi_vector_model.hh"

namespace tree_model {

  QPMI_Vector_Model::QPMI_Vector_Model(std::vector<QPersistentModelIndex> &&v,
      QObject *parent)
    :
      QAbstractTableModel(parent),
      v_(std::move(v))
  {
    if (!v_.empty() && v_[0].model())
      model_columns_ = v_[0].model()->columnCount();
  }

  int QPMI_Vector_Model::rowCount(const QModelIndex &parent) const
  {
    return int(v_.size());
  }
  int QPMI_Vector_Model::columnCount(const QModelIndex &parent) const
  {
    return model_columns_;
  }
  QVariant QPMI_Vector_Model::data(const QModelIndex &i, int role) const
  {
    if (i.isValid() && i.column() >= 0 && i.column() < model_columns_ 
        && i.row() >=0 && i.row() < int(v_.size())) {
      if (role == Qt::UserRole)
        return QVariant(
            QModelIndex(v_[i.row()].sibling(v_[i.row()].row(), i.column())));
      if (role != Qt::EditRole)
        return v_[i.row()].sibling(v_[i.row()].row(), i.column()).data(role);
    }
    return QVariant();
  }

  QVariant QPMI_Vector_Model::headerData(int section,
      Qt::Orientation orientation, int role) const
  {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
      switch (section) {
        case 0: return tr("Key");
        case 1: return tr("Value");
      }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
  }


} // tree_model
