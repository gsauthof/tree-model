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
#ifndef EDITOR_CONTROLLER_HH
#define EDITOR_CONTROLLER_HH

#include <QAbstractItemModel>
#include <QString>


namespace tree_model {
  class Base;
  class Recorder;
}
namespace editor {

  class File_Type;
  namespace command {
    class New;
    class Async_Open;
    class Async_Save;
    class Remove;
  }

  class Controller : public QObject {
    Q_OBJECT

    public:
      explicit Controller(QObject *parent = nullptr);

      QAbstractItemModel *item_tree_model();
      void request_empty_model();

    public slots:
      void open(const QString &filename);
      void save();
      void remove(const QModelIndexList &l);
      void undo();
      void redo();
    protected slots:
      void set_item_tree_model(QAbstractItemModel *model);
      void set_tree_model(tree_model::Base *model);

    private:
      command::New         *new_             {nullptr};
      command::Async_Open  *open_            {nullptr};
      command::Async_Save  *save_            {nullptr};
      command::Remove      *remove_          {nullptr};

      QAbstractItemModel   *item_tree_model_ {nullptr};
      tree_model::Base     *tree_model_      {nullptr};
    protected:
      tree_model::Recorder *recorder_        {nullptr};

    signals:
      void file_opened(const QString &filename);
      void file_type_opened(const File_Type &file_type);
      void saved(const QString &filename);
      void item_tree_model_created(QAbstractItemModel *model);
      void tree_model_created(tree_model::Base *model);
      void rewind_enabled(bool);
      void forward_enabled(bool);
      void model_changed(bool);

      void msg_produced(const QString &msg);

  };

}

#endif
