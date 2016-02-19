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

#include <QMessageBox>
#include <QClipboard>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

namespace editor {

  Main_Window::Main_Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Main_Window)
  {
    ui->setupUi(this);
    recent_menu_ = new Recent_Menu(ui->menu_File);
    ui->menu_File->insertMenu(ui->save_action, recent_menu_);
    setup_shortcuts();
    setWindowTitle(tr("unnamed[*]"));

    connect(ui->close_action, &QAction::triggered,
        this, &Main_Window::close);

    connect(ui->about_action, &QAction::triggered,
        [this](){
          QMessageBox::about(this, tr("About..."),
              tr("Hierarchical tree model editor example."));
        });
    connect(ui->clear_action, &QAction::triggered,
        [this](){
          QApplication::clipboard()->clear();
          QApplication::clipboard()->clear(QClipboard::Selection);
        });
    connect(ui->edit_action, &QAction::triggered,
        &ui->widget->tree_view(), &Tree_View::trigger_edit);
    connect(ui->add_child_action, &QAction::triggered,
        &ui->widget->tree_view(), &Tree_View::trigger_add_child);
    connect(ui->add_sibling_action, &QAction::triggered,
        &ui->widget->tree_view(), &Tree_View::trigger_add_sibling);
    connect(ui->remove_action, &QAction::triggered,
        &ui->widget->tree_view(), &Tree_View::trigger_remove);

    ui->copy_action->setEnabled(false);
    connect(ui->widget, &Tree_Widget::something_selected,
        ui->copy_action, &QAction::setEnabled);
    ui->cut_action->setEnabled(false);
    connect(ui->widget, &Tree_Widget::something_selected,
        ui->cut_action, &QAction::setEnabled);
    ui->paste_as_child_action->setEnabled(false);
    connect(ui->widget, &Tree_Widget::something_selected,
        ui->paste_as_child_action, &QAction::setEnabled);
    ui->display_subtree_action->setEnabled(false);
    connect(ui->widget, &Tree_Widget::something_selected,
        ui->display_subtree_action, &QAction::setEnabled);
    ui->remove_action->setEnabled(false);
    connect(ui->widget, &Tree_Widget::something_selected,
        ui->remove_action, &QAction::setEnabled);
    ui->edit_action->setEnabled(false);
    connect(ui->widget, &Tree_Widget::something_selected,
        ui->edit_action, &QAction::setEnabled);
    ui->add_sibling_action->setEnabled(false);
    connect(ui->widget, &Tree_Widget::something_selected,
        ui->add_sibling_action, &QAction::setEnabled);
    ui->add_child_action->setEnabled(false);
    connect(ui->widget, &Tree_Widget::something_selected,
        ui->add_child_action, &QAction::setEnabled);
  }

  Recent_Menu &Main_Window::recent_menu()
  {
    return *recent_menu_;
  }
  Tree_Widget &Main_Window::tree_widget()
  {
    return *ui->widget;
  }
  QAction &Main_Window::new_action()
  {
    return *ui->new_action;
  }
  QAction &Main_Window::open_action()
  {
    return *ui->open_action;
  }
  QAction &Main_Window::save_action()
  {
    return *ui->save_action;
  }
  QAction &Main_Window::save_as_action()
  {
    return *ui->save_as_action;
  }
  QAction &Main_Window::save_a_copy_action()
  {
    return *ui->save_a_copy_action;
  }
  QAction &Main_Window::undo_action()
  {
    return *ui->undo_action;
  }
  QAction &Main_Window::redo_action()
  {
    return *ui->redo_action;
  }
  QAction &Main_Window::cut_action()
  {
    return *ui->cut_action;
  }
  QAction &Main_Window::copy_action()
  {
    return *ui->copy_action;
  }
  QAction &Main_Window::paste_action()
  {
    return *ui->paste_action;
  }
  QAction &Main_Window::paste_as_child_action()
  {
    return *ui->paste_as_child_action;
  }
  QAction &Main_Window::display_subtree_action()
  {
    return *ui->display_subtree_action;
  }
  QAction &Main_Window::quit_action()
  {
    return *ui->quit_action;
  }

  void Main_Window::update_window_title(const QString &filename)
  {
    setWindowTitle(tr("%1[*]").arg(filename));
  }

  void Main_Window::setup_shortcuts()
  {
    // it's a pitty that Qt Creator does not support
    // adding standard key sequences ... cf.
    // https://bugreports.qt.io/browse/QTBUG-1278
    ui->open_action->setShortcut(QKeySequence::Open);
    ui->save_action->setShortcut(QKeySequence::Save);
    ui->save_as_action->setShortcut(QKeySequence::SaveAs);
    //ui->save_a_copy_action->setShortcut(...);
    ui->quit_action->setShortcut(QKeySequence::Quit);
    ui->undo_action->setShortcut(QKeySequence::Undo);
    ui->redo_action->setShortcut(QKeySequence::Redo);
    ui->copy_action->setShortcut(QKeySequence::Copy);
    ui->cut_action->setShortcut(QKeySequence::Cut);
    ui->paste_action->setShortcut(QKeySequence::Paste);
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

}
