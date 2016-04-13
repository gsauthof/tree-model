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

#ifndef TREE_MODEL_QPMI_VECTOR_MODEL_HH
#define TREE_MODEL_QPMI_VECTOR_MODEL_HH

#include <QAbstractTableModel>
#include <QPersistentModelIndex>
#include <vector>


namespace tree_model {

  class QPMI_Vector_Model : public QAbstractTableModel {
    public:
      QPMI_Vector_Model(std::vector<QPersistentModelIndex> &&v,
          QObject *parent = 0);

      int rowCount(const QModelIndex &parent = QModelIndex()) const override;
      int columnCount(const QModelIndex &parent = QModelIndex()) const override;
      QVariant data(const QModelIndex &index,
          int role = Qt::DisplayRole) const override;

      QVariant headerData(int section, Qt::Orientation orientation,
          int role = Qt::DisplayRole) const override;
    private:
      std::vector<QPersistentModelIndex> v_;
      int model_columns_ {2};
  };

} // tree_model

#endif // TREE_MODEL_QPMI_VECTOR_MODEL_HH
