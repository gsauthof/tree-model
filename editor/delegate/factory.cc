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

#include "factory.hh"

#include <editor/delegate/editor/local_time.hh>

#include <QItemEditorFactory>
#include <QItemEditorCreatorBase>
#include <QStandardItemEditorCreator>
#include <QMetaType>
#include <QDebug>

namespace editor {
  namespace delegate {

    namespace factory {

      // We assume that the factory returned by defaultFactory()
      // is not free'd.
      // This is the case for the default factory installed by Qt 5
      // (which lives as static object in a member function).
      // Note: this is not the case anymore after
      // QItemEditorFactory::setDefaultFactory() was called. Then, the
      // installed factory is deleted by any following setDefaultFactory()
      // call.
      Factory::Factory()
        :
          default_qt_factory_(QItemEditorFactory::defaultFactory())
      {
      }

      // We do it like this because (as of Qt 5.5) there is no way
      // to obtain a non-const reference to the default installed
      // factory (QItemEditorFactory::defaultFactory() returns a const
      // pointer). And the QItemEditorFactory base class does NOT
      // set up any default editors as part of its construction.
      // Instead, Qt installs QDefaultItemEditorFactory (which
      // extends QItemEditorFactory) - which is a private Qt class
      // (cf. qtbase/src/widgets/itemviews/qitemeditorfactory.cpp).
      QWidget *Factory::createEditor(int userType, QWidget *parent) const
      {
        auto a = QItemEditorFactory::createEditor(userType, parent);
        if (a)
          return a;
        if (default_qt_factory_)
          return default_qt_factory_->createEditor(userType, parent);
        return nullptr;
      }

      void setup()
      {
        // This would yield an empty factory, i.e. a factory without
        // any default editors.
        // QItemEditorFactory *factory = new QItemEditorFactory;
        QItemEditorFactory *factory = new Factory;

        QItemEditorCreatorBase *lte_creator =
            new QStandardItemEditorCreator<
              ::editor::delegate::editor::Local_Time>();

        // cf. Q_DECLARE_METATYPE() in editor/delegate/value/local_time.hh
        // the strings must match exactly - i.e. if the macro argument
        // starts with a double-colon, the query string has to start with it,
        // as well. Otherwise, type() returns unknown type (0)
        // qDebug() << "Type ID:"
        //  << QMetaType::type("::editor::delegate::value::Local_Time");
        factory->registerEditor(
            QMetaType::type("::editor::delegate::value::Local_Time"),
            lte_creator);

        QItemEditorFactory::setDefaultFactory(factory);
      }

    } // factory
  } // delegate
} // editor


