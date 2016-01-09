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
#include "test.hh"

#include <stdlib.h>
#include <boost/filesystem.hpp>

using namespace std;

namespace test {
  namespace path {

    const std::string &in()
    {
      static string p;
      if (p.empty()) {
        auto r = getenv("TEST_IN");
        if (r)
          p = r;
        else
          p = "../test/in";
        p = boost::filesystem::absolute(boost::filesystem::path(p)).generic_string();
      }
      return p;
    }

    const std::string &out()
    {
      static string p;
      if (p.empty()) {
        auto r = getenv("TEST_OUT");
        if (r)
          p = r;
        else
          p = "out";
        p = boost::filesystem::absolute(boost::filesystem::path(p)).generic_string();
      }
      return p;
    }

  }
}
