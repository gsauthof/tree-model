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

#include <ixxx/ixxx.hh>
#include <ixxx/ansi.hh>
#include <ixxx/posix.hh>

#include <QString>
#include <QSettings>
#include <QCoreApplication>

#include <string.h>
#include <string>
#include <sstream>

using namespace std;

namespace editor {

  namespace {
    namespace key {
      static const char asn1[] = "asn1";
      static const char path[] = "path";
    }
  }

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

  void setup_asn1_path()
  {
    // cf. libxfsx/xfsx/detector:cc, default_asn_search_path()
    // for the complete story
    string asn1_path;
    try {
      asn1_path = ixxx::ansi::getenv("ASN1_PATH");
    } catch (const ixxx::runtime_error &e) {
    }
    if (asn1_path.empty()) {
      QSettings s;
      s.beginGroup(key::asn1);
      string spath(s.value(key::path).toString().toStdString());
      s.endGroup();
      ostringstream o;
      if (!spath.empty())
        o << spath << ':';
      string dir_path(QCoreApplication::applicationDirPath().toStdString());
      o << dir_path
#if (defined(__MINGW32__) || defined(__MINGW64__))
        << ':' << dir_path << "/config"
#else
        << ':' << dir_path << "/../share/xfsx/asn1"
#endif
        ;
      asn1_path = o.str();
      ixxx::posix::setenv("ASN1_PATH", asn1_path, true);
    }
  }

} // namespace editor

