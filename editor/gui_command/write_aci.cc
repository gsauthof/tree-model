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

#include "write_aci.hh"

#include <editor/traverser/check_cancel.hh>

#include <QMessageBox>
#include <QProgressDialog>
#include <QDebug>

#include <exception>

using namespace std;

namespace editor {

  namespace gui_command {

    Write_ACI::Write_ACI(QWidget *p)
      :
        editor::command::Write_ACI(static_cast<QObject*>(p)),
        parent_widget_(p)
    {
    }

    void Write_ACI::write()
    {
      unsigned cnt = 0;
      // We use a QProgressDialog here -  and not a worker thread -
      // because the command traverses and changes the model.
      // The model is also used by the view and is not thread safe.
      // Even only doing the traverse part in a worker thread would not
      // be safe because read-only model access may also change the
      // internal cache data structures of the model.
      // Alternative:
      // - directly traverse over the XML tree in a worker thread
      // - while doing so put the model into read-only mode
      // - transfer the newly computed ACI info to the GUI-thread
      // - do the rewrite of the ACI in the GUI-thread
      QProgressDialog pd(tr("Computing Audit Control Info..."), tr("&Cancel"),
          0, 100, parent_widget_);
      // Application modality is the default with QProgressDialog
      // setting Qt::WindowModal is tempting but:
      // - there might be subtree windows open where the user is
      //   able to change the tree while aci is traversing, thus
      //   such windows would to be put into read-only mode while
      //   traversing
      // - aci computation should be very fast, thus, it is not
      //   worth it
      pd.setMinimumDuration(1300);
      set_cancel_function([&cnt, &pd]() {
          pd.setValue(cnt++ % 100);
          return pd.wasCanceled();
          });
      try {
        editor::command::Write_ACI::write();
      } catch (const editor::traverser::Cancel_Exception &e) {
        emit was_canceled();
      } catch (const std::exception &e) {
        pd.setValue(100);
        QMessageBox::critical(parent_widget_,
            tr("Write Audit Control Info Error"),
            e.what());
      }
      pd.setValue(100);
    }


  } // gui_command

} // editor
