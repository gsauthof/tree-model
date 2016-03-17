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
#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

#include <editor/file_type.hh>

#include <QApplication>

#include <memory>
#include <utility>
#include <string.h>

using namespace std;

static pair<int, unique_ptr<char*[]> > create_qtest_argv(int ac, char **av)
{
  unique_ptr<char*[]> argv(new char*[ac]);
  int argc = 0;
  if (ac)
    argv[argc++] = av[0];
  for (int i = 1; i < ac; ++i) {
    if (!strcmp(av[i], "-platform")) {
      argv[argc++] = av[i];
      ++i;
      if (i < ac)
        argv[argc++] = av[i];
      break;
    }
  }
  return make_pair(argc, std::move(argv));
}

static pair<int, unique_ptr<char*[]> > create_catch_argv(int ac, char **av)
{
  unique_ptr<char*[]> argv(new char*[ac]);
  int argc = 0;
  if (ac)
    argv[argc++] = av[0];
  for (int i = 1; i < ac; ++i) {
    if (!strcmp(av[i], "-platform")) {
      ++i;
      continue;
    }
    argv[argc++] = av[i];
  }
  return make_pair(argc, std::move(argv));
}

int main(int argc, char **argv)
{
  auto qtest_argv = create_qtest_argv(argc, argv);
  auto catch_argv = create_catch_argv(argc, argv);

  // without QApplication, widgets can segfault
  // on uninitialized singleton data ...
  // Also, QApplication must be constructed once, constructing it e.g.
  // in each test case yields segfaults ...
  QApplication app(qtest_argv.first, qtest_argv.second.get());
  app.setAttribute(Qt::AA_Use96Dpi, true);
  editor::File_Type::register_meta_type();
  int result = Catch::Session().run(catch_argv.first, catch_argv.second.get());
  return result;
}
