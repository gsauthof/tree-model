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

#include "value.hh"

#include "model_validator.hh"
#include "int64_validator.hh"

#include <editor/file_type.hh>

#include <grammar/grammar.hh>
#include <grammar/constraint/variant.hh>

#include <QDebug>
#include <QLineEdit>
#include <QCompleter>
#include <QRegularExpressionValidator>
#include <QStringListModel>


// XXX query qsettings if validation should be enabled
// XXX make it configurable via menu 'Settings'

namespace editor {
  namespace delegate {

    class Value::Private {
      public:
        std::unordered_map<std::string, grammar::Constraint::Variant>
          constraints;

        void read_constraints(const std::deque<std::string> &filenames);

      private:
    };

    Value::Value(QObject *parent)
      :
        QStyledItemDelegate(parent),
        priv_(std::make_unique<Value::Private>())
    {
    }
    Value::~Value() =default;

    struct Create_Completer {
      using result_type = QCompleter*;
      QObject *parent_ {nullptr};
      Create_Completer(QObject *parent) : parent_(parent) { }
      QCompleter *operator()(const grammar::Constraint::Base &b) const
      {
        return nullptr;
      }
      QCompleter *operator()(const grammar::Constraint::Enum &b) const
      {
        auto completer = new QCompleter(parent_);
        QStringList l;
        for (auto v : b.values())
          l << QString(v.c_str());
        l.sort();
        auto model = new QStringListModel(l, parent_);
        completer->setModel(model);
        completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
        return completer;
      }
    };
    struct Create_Validator {
      using result_type = QValidator*;
      QObject *parent_ {nullptr};
      Create_Validator(QObject *parent) : parent_(parent) { }
      QValidator *operator()(const grammar::Constraint::Base &b) const
      {
        return nullptr;
      }
      QValidator *operator()(const grammar::Constraint::Domain &b) const
      {
        auto validator = new Int64_Validator(b.min(), b.max(), parent_);
        return validator;
      }
      QValidator *operator()(const grammar::Constraint::Pattern &b) const
      {
        auto validator = new QRegularExpressionValidator(
            QRegularExpression(b.str().c_str()), parent_);
        return validator;
      }
      QValidator *operator()(const grammar::Constraint::Size &b) const
      {
        // possible optimization:
        // subclass QValidator and directly test size there
        auto validator = new QRegularExpressionValidator(
            QRegularExpression(
              QString(".{%1,%2}").arg(b.range().first).arg(b.range().second)),
            parent_);
        return validator;
      }
      QValidator *operator()(const grammar::Constraint::Enum &b) const
      {
        QStringList l;
        for (auto v : b.values())
          l << QString(v.c_str());
        l.sort();
        auto model = new QStringListModel(l, parent_);
        auto validator = new Model_Validator(*model, parent_);
        return validator;
      }
    };

    QWidget *Value::createEditor(QWidget *parent,
        const QStyleOptionViewItem &option, const QModelIndex &index)
      const
      {
        auto r = QStyledItemDelegate::createEditor(parent, option, index);
        if (!index.isValid() || !index.column())
          return r;
        // Qt uses as default editor QExpandingLineEdit, which
        // inherits QLineEdit
        auto line_edit = dynamic_cast<QLineEdit*>(r);
        if (line_edit && index.column() == 1) {
          auto tag = index.sibling(index.row(), 0)
            .data().toString().toStdString();

          auto i = priv_->constraints.find(tag);
          if (i != priv_->constraints.end()) {
            auto completer = i->second.accept(Create_Completer(line_edit));
            if (completer)
              line_edit->setCompleter(completer);
            auto validator = i->second.accept(Create_Validator(line_edit));
            if (validator)
              line_edit->setValidator(validator);
          }
        }
        return r;
      }

    void Value::Private::read_constraints(
        const std::deque<std::string> &filenames)
    {
      if (filenames.size() == 1) {
        constraints = grammar::make_constraint_map(grammar::read_constraints(
              filenames.front()));
      } else {
        constraints.clear();
        for (auto &filename : filenames) {
          auto m = grammar::make_constraint_map(grammar::read_constraints(
                filename));
          for (auto &p : m)
            constraints.insert(make_pair(p.first, std::move(p.second)));
        }
      }
    }

    void Value::read_constraints(const std::deque<std::string> &filenames)
    {
      priv_->read_constraints(filenames);
    }

    void Value::apply_file_type(const File_Type &ft)
    {
      read_constraints(ft.constraint_filenames());
    }

  } // delegate
} //editor
