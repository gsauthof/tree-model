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

#include <QMessageBox>
#include <QDebug>

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

    connect(ui->about_action, &QAction::triggered,
        [this](){
          QMessageBox::about(this, tr("About..."),
              tr("Hierarchical tree model editor example."));
        });

    // updates the window title iff it contains the [*] placeholder

    ui->copy_action->setEnabled(false);
    connect(ui->widget, &Tree_Widget::something_selected,
        ui->copy_action, &QAction::setEnabled);

  }

  Recent_Menu &Main_Window::recent_menu()
  {
    return *recent_menu_;
  }
  Tree_Widget &Main_Window::tree_widget()
  {
    return *ui->widget;
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
  QAction &Main_Window::copy_action()
  {
    return *ui->copy_action;
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
  }

  Main_Window::~Main_Window()
  {
    delete ui;
  }

  void Main_Window::display_status(const QString &msg)
  {
    statusBar()->showMessage(msg, 30000);
  }

}
