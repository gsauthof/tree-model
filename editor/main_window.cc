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
#include "main_window.hh"
#include "ui_main_window.h"
#include <editor/recent_menu.hh>
#include "tree_view.hh"
#include "qt_awesome.hh"

#include <QMessageBox>
#include <QClipboard>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>
#include <QMimeData>

namespace editor {

  namespace {
    namespace key {
      static const char main_window [] = "main_window";
      static const char geometry    [] = "geometry";
      static const char window_state[] = "window_state";
    }
  }

  Main_Window::Main_Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Main_Window)
  {
    ui->setupUi(this);
    restore_settings();
    recent_menu_ = new Recent_Menu(ui->menu_File);
    ui->menu_File->insertMenu(ui->save_action, recent_menu_);
    setup_shortcuts();
    setup_icons();
    setWindowTitle(tr("unnamed[*]"));
    setAcceptDrops(true);

    connect(ui->close_action, &QAction::triggered,
        this, &Main_Window::close);

    setup_about_action();
    setup_clear_action();
    connect_actions_to_tree_view();
    setup_selection_dependent_actions();
    forward_trigger_signals();
  }

  void Main_Window::setup_about_action()
  {
    connect(ui->about_action, &QAction::triggered,
        [this](){
          QMessageBox::about(this, tr("About..."),
              tr("Hierarchical tree model editor example."));
        });
  }

  void Main_Window::setup_clear_action()
  {
    connect(ui->clear_action, &QAction::triggered,
        [this](){
          QApplication::clipboard()->clear();
          QApplication::clipboard()->clear(QClipboard::Selection);
        });
  }

  void Main_Window::connect_actions_to_tree_view()
  {
    connect(ui->expand_some_action, &QAction::triggered, [this]() {
        ui->widget->tree_view().breadth_first_expand(20); });
    connect(ui->collapse_some_action, &QAction::triggered, [this]() {
        ui->widget->tree_view().breadth_first_collapse(100); });

    ui->widget->tree_view().set_edit_action       (ui->edit_action);
    ui->widget->tree_view().set_add_child_action  (ui->add_child_action);
    ui->widget->tree_view().set_add_sibling_action(ui->add_sibling_action);
    ui->widget->tree_view().set_remove_action     (ui->remove_action);
    ui->widget->tree_view().set_copy_action       (ui->copy_action);
    ui->widget->tree_view().set_cut_action        (ui->cut_action);
    ui->widget->tree_view().set_paste_action      (ui->paste_action);
    ui->widget->tree_view().set_paste_as_child_action(
        ui->paste_as_child_action);
  }

  void Main_Window::setup_selection_dependent_actions()
  {
    ui->display_subtree_action->setEnabled(false);
    connect(ui->widget, &Tree_Widget::something_selected,
        ui->display_subtree_action, &QAction::setEnabled);
  }

  void Main_Window::forward_trigger_signals()
  {
    connect(ui->new_action, &QAction::triggered,
        this, &Main_Window::new_triggered);
    connect(ui->open_action, &QAction::triggered,
        this, &Main_Window::open_triggered);
    connect(ui->save_action, &QAction::triggered,
        this, &Main_Window::save_triggered);
    connect(ui->save_as_action, &QAction::triggered,
        this, &Main_Window::save_as_triggered);
    connect(ui->save_a_copy_action, &QAction::triggered,
        this, &Main_Window::save_a_copy_triggered);
    connect(ui->undo_action, &QAction::triggered,
        this, &Main_Window::undo_triggered);
    connect(ui->redo_action, &QAction::triggered,
        this, &Main_Window::redo_triggered);
    connect(ui->display_subtree_action, &QAction::triggered,
        this, &Main_Window::display_subtree_triggered);
    connect(ui->write_aci_action, &QAction::triggered,
        this, &Main_Window::write_aci_triggered);
    connect(ui->find_action, &QAction::triggered,
        this, &Main_Window::find_triggered);
    connect(ui->quit_action, &QAction::triggered,
        this, &Main_Window::quit_triggered);
  }

  Recent_Menu &Main_Window::recent_menu()
  {
    return *recent_menu_;
  }
  Tree_Widget &Main_Window::tree_widget()
  {
    return *ui->widget;
  }

  void Main_Window::update_window_title(const QString &filename)
  {
    if (filename.isEmpty())
      setWindowTitle(tr("unnamed[*]").arg(filename));
    else
      setWindowTitle(tr("%1[*]").arg(filename));
  }

  void Main_Window::setup_shortcuts()
  {
    // it's a pitty that Qt Creator does not support
    // adding standard key sequences ... cf.
    // https://bugreports.qt.io/browse/QTBUG-1278
    ui->open_action         ->setShortcut(QKeySequence::Open);
    ui->save_action         ->setShortcut(QKeySequence::Save);
    ui->save_as_action      ->setShortcut(QKeySequence::SaveAs);
    //ui->save_a_copy_action->setShortcut(...);
    ui->quit_action         ->setShortcut(QKeySequence::Quit);
    ui->undo_action         ->setShortcut(QKeySequence::Undo);
    ui->redo_action         ->setShortcut(QKeySequence::Redo);
    ui->copy_action         ->setShortcut(QKeySequence::Copy);
    ui->cut_action          ->setShortcut(QKeySequence::Cut);
    ui->paste_action        ->setShortcut(QKeySequence::Paste);

    // usually that maps to the DEL key
    // even without that shortcut, the QTreeView interprets by
    // default Delete (DEL) and removes the selected rows
    // from the model
    ui->remove_action       ->setShortcut(QKeySequence::Delete);
  }
  void Main_Window::setup_icons()
  {
    // cf. http://fontawesome.io/icons/ for a list
    ui->new_action            ->setIcon(fa_instance()->icon(fa::plus));
    ui->open_action           ->setIcon(fa_instance()->icon(fa::fileo));
    ui->save_action           ->setIcon(fa_instance()->icon(fa::floppyo));
    ui->save_as_action        ->setIcon(fa_instance()->icon(fa::filetext));
    ui->undo_action           ->setIcon(fa_instance()->icon(fa::undo));
    ui->redo_action           ->setIcon(fa_instance()->icon(fa::repeat));
    ui->cut_action            ->setIcon(fa_instance()->icon(fa::scissors));
    ui->copy_action           ->setIcon(fa_instance()->icon(fa::clone));
    ui->paste_action          ->setIcon(fa_instance()->icon(fa::clipboard));
    ui->display_subtree_action->setIcon(fa_instance()->icon(fa::externallink));

    //ui->remove_action         ->setIcon(fa_instance()->icon(fa::remove));
    ui->remove_action         ->setIcon(fa_instance()->icon(fa::trash));
    ui->add_child_action      ->setIcon(fa_instance()->icon(fa::plussquareo));
    ui->add_sibling_action    ->setIcon(fa_instance()->icon(fa::pluscircle));
    ui->close_action          ->setIcon(fa_instance()->icon(fa::times));
    ui->quit_action           ->setIcon(fa_instance()->icon(fa::signout));
  }


  Main_Window::~Main_Window()
  {
    delete ui;
  }

  void Main_Window::display_status(const QString &msg)
  {
    statusBar()->showMessage(msg, 30000);
  }

  void Main_Window::closeEvent(QCloseEvent *event)
  {
    store_settings();
    if (isWindowModified()) {
      QMessageBox q(this);
      q.setText(tr("The file has been modified."));
      q.setInformativeText(tr("Do you want to save your changes?"));
      q.setStandardButtons(/*QMessageBox::Save |*/ QMessageBox::Discard
          | QMessageBox::Cancel);
      q.setDefaultButton(QMessageBox::Discard);
      q.setDetailedText(windowTitle());
      int r = q.exec();
      switch (r) {
        case QMessageBox::Cancel:
          event->ignore();
          break;
          /*
        case QMessageBox::Save:
          // TODO: worth it?
          // would also need to handle save-as situations
          // and write errors ...
          // thus, just calling an async-save is probably
          // not necessarily sufficient
          ui->save_action->trigger();
          event->accept();
          break;
          */
        default:
          event->accept();
      }
    } else {
      event->accept();
    }
  }
  void Main_Window::store_settings()
  {
    QSettings s;
    s.beginGroup(key::main_window);
    s.setValue(key::geometry, saveGeometry());
    s.setValue(key::window_state, saveState());
    s.endGroup();
  }
  void Main_Window::restore_settings()
  {
    QSettings s;
    s.beginGroup(key::main_window);
    restoreGeometry(s.value(key::geometry).toByteArray());
    restoreState(s.value(key::window_state).toByteArray());
    s.endGroup();
  }

  void Main_Window::enable_undo(bool b)
  {
    ui->undo_action->setEnabled(b);
  }
  void Main_Window::enable_redo(bool b)
  {
    ui->redo_action->setEnabled(b);
  }


  void Main_Window::dragEnterEvent(QDragEnterEvent *e)
  {
    if (e->mimeData()->hasUrls())
      e->acceptProposedAction();
  }
  /*
  void Main_Window::dragLeaveEvent(QDragLeaveEvent *e)
  {
  }
  */
  void Main_Window::dragMoveEvent(QDragMoveEvent *e)
  {
    if (e->mimeData()->hasUrls())
      e->acceptProposedAction();
  }
  void Main_Window::dropEvent(QDropEvent *e)
  {
    QList<QUrl> urls;
    for (auto &url : e->mimeData()->urls())
      if (url.isLocalFile())
        urls.push_back(url);
    emit open_urls_requested(urls);
  }

}
