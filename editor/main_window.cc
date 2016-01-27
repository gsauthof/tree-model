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
#include <editor/gui_controller.hh>
#include <editor/recent_menu.hh>

#include <QMessageBox>
#include <QDebug>

namespace editor {

  Main_Window::Main_Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Main_Window),
    controller_(new Gui_Controller(this))
  {
    ui->setupUi(this);
    ui->widget->set_controller(controller_);
    auto recent_menu = new Recent_Menu(ui->menu_File);
    ui->menu_File->insertMenu(ui->action_Save, recent_menu);
    setup_shortcuts();
    setWindowTitle(tr("unnamed[*]"));

    connect(ui->action_Open, &QAction::triggered,
        controller_, &Gui_Controller::select_open);
    connect(ui->action_Save, &QAction::triggered,
        controller_, &Gui_Controller::save);
    connect(ui->action_Save_As, &QAction::triggered,
        controller_, &Gui_Controller::select_save);
    connect(ui->action_Save_A_Copy, &QAction::triggered,
        controller_, &Gui_Controller::select_save_copy);

    connect(ui->action_About, &QAction::triggered,
        [this](){
          QMessageBox::about(this, tr("About..."),
              tr("Hierarchical tree model editor example."));
        });

    connect(controller_, &Controller::rewind_enabled,
        ui->action_Undo, &QAction::setEnabled);
    connect(controller_, &Controller::forward_enabled,
        ui->action_Redo, &QAction::setEnabled);
    // updates the window title iff it contains the [*] placeholder
    connect(controller_, &Controller::model_changed,
        this, &QMainWindow::setWindowModified);

    connect(ui->action_Undo, &QAction::triggered,
        controller_, &Controller::undo);
    connect(ui->action_Redo, &QAction::triggered,
        controller_, &Controller::redo);
    connect(ui->action_copy, &QAction::triggered,
        controller_, &Gui_Controller::clipboard_copy);

    connect(controller_, &Gui_Controller::msg_produced,
        this, &Main_Window::display_status);
    connect(controller_, &Controller::file_opened,
        this, &Main_Window::update_window_title);
    connect(controller_, &Controller::saved,
        this, &Main_Window::update_window_title);

    connect(controller_, &Controller::file_opened,
        recent_menu, &Recent_Menu::register_file_open);
    connect(recent_menu, &Recent_Menu::chosen,
        controller_, &Gui_Controller::open);
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
    ui->action_Open->setShortcut(QKeySequence::Open);
    ui->action_Save->setShortcut(QKeySequence::Save);
    ui->action_Save_As->setShortcut(QKeySequence::SaveAs);
    //ui->action_Save_A_Copy->setShortcut(...);
    ui->action_Quit->setShortcut(QKeySequence::Quit);
    ui->action_Undo->setShortcut(QKeySequence::Undo);
    ui->action_Redo->setShortcut(QKeySequence::Redo);
    ui->action_copy->setShortcut(QKeySequence::Copy);
  }

  Main_Window::~Main_Window()
  {
    delete ui;
  }

  void Main_Window::open(const QString &filename)
  {
    controller_->open(filename);
  }
  void Main_Window::display_status(const QString &msg)
  {
    statusBar()->showMessage(msg, 30000);
  }

}
