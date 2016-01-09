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
#include "arguments.hh"

#include <string.h>

namespace editor {

Arguments::Arguments(QObject *parent) : QObject(parent)
{

}

Arguments::Arguments(int argc, char **argv, QObject *parent)
  :
    QObject(parent),
    argc_(argc),
    argv_(argv)
{

}

void Arguments::parse()
{
  for (int i = 1; i < argc_; ++i) {
    if (!strcmp(argv_[i], "--")) {
      ++i;
      for (; i<argc_; ++i)
        positionals.push_back(QString(argv_[i]));
    } else if (*argv_[i] != '-') {
      positionals.push_back(QString(argv_[i]));
    }
  }
  if (!positionals.empty())
    input_filename = positionals.front();
  if (!input_filename.isEmpty())
    emit open_triggered(input_filename);
}

} // namespace editor

