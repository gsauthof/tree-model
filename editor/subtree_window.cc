#include "subtree_window.hh"
#include "ui_subtree_window.h"

namespace editor {

Subtree_Window::Subtree_Window(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Subtree_Window)
{
  ui->setupUi(this);
}

Tree_Widget &Subtree_Window::tree_widget()
{
  return *ui->widget;
}

Subtree_Window::~Subtree_Window()
{
  delete ui;
}

} // namespace editor
