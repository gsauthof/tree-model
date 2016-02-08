#ifndef EDITOR_SLIDER_BAR_HH
#define EDITOR_SLIDER_BAR_HH

#include <QWidget>

namespace editor {

  namespace Ui {
    class Slider_Bar;
  }

  class Slider_Bar : public QWidget
  {
    Q_OBJECT

    public:
      explicit Slider_Bar(QWidget *parent = nullptr);
      ~Slider_Bar();

    public slots:
      void init(int pos, int max);

    signals:
      void jump_requested(int pos);

    private:
      Ui::Slider_Bar *ui;

      void setup_slider_machine();

    private slots:
      void jump_to_edit_line_rank();

  };


} // namespace editor
#endif // EDITOR_SLIDER_BAR_HH
