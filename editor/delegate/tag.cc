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

#include "tag.hh"

#include <grammar/grammar.hh>

#include <QDebug>
#include <QLineEdit>
#include <QCompleter>
#include <QStringListModel>

namespace editor {
  namespace delegate {

    QWidget *Tag::createEditor(QWidget *parent,
        const QStyleOptionViewItem &option, const QModelIndex &index)
      const
      {
        auto r = QStyledItemDelegate::createEditor(parent, option, index);
        if (!index.isValid() || index.column())
          return r;
        // Qt uses as default editor QExpandingLineEdit, which
        // inherits QLineEdit
        auto line_edit = dynamic_cast<QLineEdit*>(r);
        if (line_edit && !names_.isEmpty()) {
          auto completer = new QCompleter(line_edit);
          completer->setModel(names_model_);
          completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
          line_edit->setCompleter(completer);
          // XXX set validator
        }
        return r;
      }

      void Tag::apply_grammar(const grammar::Grammar *gg)
      {
        if (!gg) {
          names_.clear();
          return;
        }
        const grammar::Grammar &g = *gg;
        names_.clear();
        for (auto &ntp : g.nts()) {
          auto &nt = *ntp;
          // we don't consider nt.coord().klasse() here
          if (nt.coord().initialized())
            names_ << nt.name().c_str();
        }
        for (auto &t : g.terminals()) {
          auto &terminal = *t;
          names_ << terminal.name().c_str();
        }
        names_.sort();
        delete names_model_;
        names_model_ = new QStringListModel(names_, this);
      }

  } // delegate
} //editor
