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

#ifndef EDITOR_COMMAND_READ_GRAMMAR_HH
#define EDITOR_COMMAND_READ_GRAMMAR_HH

#include <QObject>
#include <QString>

#include <memory>

namespace grammar {
  class Grammar;
}

namespace editor {
  class File_Type;
  namespace command {

    class Read_Grammar : public QObject {
      Q_OBJECT
      private:
        std::unique_ptr<grammar::Grammar> grammar_;
      public:
        using QObject::QObject;
        ~Read_Grammar();

      public slots:
        void set_file_type(const File_Type &ft);
      signals:
        void grammar_read(const grammar::Grammar *grammar);
        
    };

  } // command
} // editor

#endif // EDITOR_COMMAND_READ_GRAMMAR_HH
