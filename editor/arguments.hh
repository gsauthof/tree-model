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
#ifndef EDITOR_ARGUMENTS_HH
#define EDITOR_ARGUMENTS_HH

#include <QObject>

#include <deque>

namespace editor {

class Arguments : public QObject
{
    Q_OBJECT
  public:
    explicit Arguments(QObject *parent = nullptr);
    Arguments(int argc, char **argv, QObject *parent = nullptr);

    std::deque<QString> positionals;
    QString input_filename;
  signals:
    void open_triggered(const QString &filename);

  public slots:
    void parse();
  private:
    int argc_ {0};
    char **argv_ {nullptr};
};

} // namespace editor

#endif // EDITOR_ARGUMENTS_HH
