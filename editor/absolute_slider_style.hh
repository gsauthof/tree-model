#ifndef ABSOLUTE_SLIDER_STYLE_HH
#define ABSOLUTE_SLIDER_STYLE_HH

#include <QProxyStyle>


namespace editor {

  // from: http://stackoverflow.com/questions/11132597/qslider-mouse-direct-jump
  class Absolute_Slider_Style : public QProxyStyle
  {
    public:
      using QProxyStyle::QProxyStyle;

      int styleHint(QStyle::StyleHint hint, const QStyleOption* option = 0,
          const QWidget* widget = 0, QStyleHintReturn* returnData = 0) const;

  };
}

#endif
