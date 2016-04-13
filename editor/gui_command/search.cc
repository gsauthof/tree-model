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

#include "search.hh"

#include <editor/child_dialog.hh>
#include <editor/progress_dialog.hh>
#include <editor/result_window.hh>
#include <editor/table_view.hh>

#include <tree_model/qpmi_vector_model.hh>
#include <tree_model/item_adaptor.hh>
#include <tree_model/protected_item_model.hh>

#include <QDebug>
#include <deque>

using namespace std;

namespace editor {

  namespace gui_command {

    Search::Search(QWidget *parent_widget)
      :
        Async_Search(static_cast<QObject*>(parent_widget)),
        parent_widget_(parent_widget)
    {
    }
    void Search::display()
    {
      auto pmodel = dynamic_cast<tree_model::Protected_Item_Model<
        tree_model::Item_Adaptor>*>(model_);
      if (!pmodel)
        return;
      editor::Child_Dialog d(parent_widget_);
      d.setWindowTitle(tr("Search"));

      if (d.exec()) {
        if (d.key().isEmpty())
          return;
        Progress_Dialog pd(tr("Searching ..."), QString(), parent_widget_);
        connect(this, &Search::finished, &pd, &Progress_Dialog::finish);

        // since the worker thread is traversing the tree data
        // structure we have to make sure that no node is
        // invalidated - in parallel
        pmodel->set_read_only(true);
        if (d.value().isEmpty())
          search_key(d.key());
        else
          search(d.key(), d.value());
        pd.wait();
        pmodel->set_read_only(false);

        deque<const xmlNode*> l;
        move_result_to(l);
        auto v = mk_qpmi_vector(l, *model_);
        if (v.empty()) {
          emit msg_produced(tr("Nothing found"));
        } else {
          auto w = new Result_Window(parent_widget_);
          w->setWindowTitle(tr("Search Results"));
          // XXX do the same for display subtree command ...
          // we set this attribute because we want to free the
          // memory of closed windows as soon as possible;
          // otherwise they aren't freed until the parent is
          // destructed - which only happens at program exit
          w->setAttribute(Qt::WA_DeleteOnClose);
          w->table_view().setModel(new tree_model::QPMI_Vector_Model(
                std::move(v), w));
          connect(&w->table_view(), &Table_View::current_changed,
              this, &Search::translate_current);

          emit result_window_created(w);
          w->show();
        }
      }
    }
    void Search::translate_current(const QModelIndex &i,
        const QModelIndex &j)
    {
      emit current_changed(i.data(Qt::UserRole).toModelIndex(),
                           j.data(Qt::UserRole).toModelIndex());
    }
    void Search::set_model(QAbstractItemModel *model)
    {
      model_ = model;
    }

  } // gui_command

} // editor
