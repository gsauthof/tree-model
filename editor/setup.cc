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

#include "setup.hh"

#include "arguments.hh"
#include "file_type.hh"
#include <editor/delegate/value/local_time.hh>
#include <editor/delegate/factory.hh>
#include "qt_awesome.hh"

#include <QApplication>
#include <QStyle>
#include <QDebug>

#include <libxml/parser.h>

#include <stdexcept>

using namespace std;

namespace editor {

  class Setup::Private {
    public:
      Private();

      void workaround_gtk_plus_style_transparency_bug();
    private:
      unique_ptr<QtAwesome> fai;
  };

  Setup::Private::Private()
    :
      fai(editor::fa_instance())
  {
    workaround_gtk_plus_style_transparency_bug();
  }

  // See also:
  // http://stackoverflow.com/questions/19549740/how-to-clear-background-in-qtreeview-during-item-edit
  // https://bugreports.qt.io/browse/QTBUG-31509
  void Setup::Private::workaround_gtk_plus_style_transparency_bug()
  {
    if (QApplication::style()
        && QApplication::style()->objectName() == "gtk+"
        && qApp->styleSheet().isEmpty()) {
      qApp->setStyleSheet(
          "QLineEdit, QAbstractSpinBox { background-color: white; }");
    }
  }

  Setup::Setup()
    :
      d_(std::make_unique<Setup::Private>())
  {
    static unsigned cnt = 0;
    if (cnt)
      throw logic_error("Setup should only exist once, ideally in main()");
    ++cnt;

    // cf. http://www.xmlsoft.org/threads.html
    LIBXML_TEST_VERSION
    // LIBXML_TEST_VERSION already calls xmlInitParser()
    // xmlInitParser();


    if (!QApplication::instance())
      throw logic_error("QApplication must be allocated"
          " before constructing the Setup object");

    editor::File_Type::register_meta_type();
    editor::delegate::value::Local_Time::register_meta_type();
    editor::delegate::factory::setup();

    editor::setup_asn1_path();
  }

  Setup::~Setup()
  {
    // perhaps necessary if leaks show up in the leak detector
    // xmlCleanupParser();
    // xmlSchemaCleanupTypes();
  }

}
