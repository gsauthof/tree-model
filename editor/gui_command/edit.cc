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
#include "edit.hh"

#include <editor/child_dialog.hh>
#include <editor/delegate/tag.hh>
#include <editor/delegate/value.hh>

#include <vector>
#include <utility>

#include <QDebug>
#include <QAbstractItemModel>


namespace editor {
  namespace gui_command {

    Edit::Edit(QWidget *parent)
      :
        QObject(static_cast<QObject*>(parent)),
        parent_widget_(parent),
        key_delegate_(new delegate::Tag(this)),
        value_delegate_(new delegate::Value(this))
    {
    }
    QWidget *Edit::create_editor(QAbstractItemDelegate *delegate,
        const QModelIndex &index)
    {
      auto widget = delegate->createEditor(nullptr,
          QStyleOptionViewItem(), index);
      if (!widget)
        return nullptr;


      delegate->setEditorData(widget, index);
      if (!(index.flags() & Qt::ItemIsEditable))
        widget->setEnabled(false);
      return widget;
    }

    void Edit::edit(const QModelIndex &index)
    {
      if (!model_)
        return;
      if (!index.isValid())
        return;

      editor::Child_Dialog d(parent_widget_);
      d.setWindowTitle(tr("Edit node"));

      auto key_index = index.sibling(index.row(), 0);
      auto value_index = index.sibling(index.row(), 1);

      auto key_widget = create_editor(key_delegate_, key_index);
      if (!key_widget)
        return;
      d.install_widget(key_widget, 0);
      auto value_widget = create_editor(value_delegate_, value_index);
      d.install_widget(value_widget, 1);

      if (d.exec()) {
        // we use Qt's meta object system to access that
        // property, because widgets like QLineEdit/QAbstractSpinBox have it,
        // but the QWidget base class hasn't
        if (key_widget->property("acceptableInput").toBool()
            && (!value_widget
              || !value_widget->isEnabled()
              || value_widget->property("acceptableInput").toBool()
              )) {
          emit begin_transaction_requested(tr("edit node"));
          key_delegate_->setModelData(key_widget, model_, key_index);
          if (value_widget && value_widget->isEnabled())
            value_delegate_->setModelData(value_widget, model_, value_index);
          emit commit_requested();
        }
      }
    }
    void Edit::set_model(QAbstractItemModel *model)
    {
      model_ = model;
    }

    void Edit::apply_grammar(const grammar::Grammar *g)
    {
      key_delegate_->apply_grammar(g);
    }
    void Edit::apply_file_type(const File_Type &ft)
    {
      value_delegate_->apply_file_type(ft);
    }

  }
}

/*

We want to reuse the delegate infrastructure we already have in
place for tree-view editing. The QDataWidgetMapper is step in
that direction, because one can connect edit widgets with
a model. But with QDataWidgetMapper, only data retrieval/storage
from/to the model is delegated - not the creation of edit
widgets. That means that the setup of validators/completers
is not shared.

Thus, the above direct usage of delegates is the better alternative.

    #include <QDataWidgetMapper>

      bool has_children = model_->hasChildren(
          model_->index(index.row(), 0, index.parent()));
      // the mapper doesn't do the disabling for us, i.e.
      // it doesn't query
      // QAbstractItemModel::flags & Qt::ItemIsEditable
      // apparently
      d.value_line().setEnabled(!has_children);

      QDataWidgetMapper mapper;
      mapper.setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
      mapper.setModel(model_);
      mapper.setRootIndex(index.parent());
      mapper.addMapping(&d.key_line(), 0);
      mapper.addMapping(&d.value_line(), 1);
      mapper.setCurrentIndex(index.row());

      if (d.exec()) {
        emit begin_transaction_requested(tr("edit node"));
        mapper.submit();
        emit commit_requested();
      }

*/
