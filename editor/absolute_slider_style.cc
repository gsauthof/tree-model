#include "absolute_slider_style.hh"


namespace editor {

  /* This style hint makes QSlider directly jump to the clicked
   * position.
   * The default behaviour is: increase by some ticks.
   */

  // from: http://stackoverflow.com/questions/11132597/qslider-mouse-direct-jump
  int Absolute_Slider_Style::styleHint(QStyle::StyleHint hint,
      const QStyleOption* option,
      const QWidget* widget, QStyleHintReturn* returnData) const
  {
    if (hint == QStyle::SH_Slider_AbsoluteSetButtons)
      return (Qt::LeftButton | Qt::MidButton | Qt::RightButton);
    return QProxyStyle::styleHint(hint, option, widget, returnData);
  }

}
