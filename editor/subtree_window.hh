#ifndef EDITOR_SUBTREE_WINDOW_HH
#define EDITOR_SUBTREE_WINDOW_HH

#include <QDialog>

namespace editor {

  class Tree_Widget;

  namespace Ui {
    class Subtree_Window;
  }

  class Subtree_Window : public QDialog
  {
    Q_OBJECT

    public:
      explicit Subtree_Window(QWidget *parent = 0);
      ~Subtree_Window();

      Tree_Widget &tree_widget();
    private:
      Ui::Subtree_Window *ui;
  };


} // namespace editor
#endif // EDITOR_SUBTREE_WINDOW_HH
