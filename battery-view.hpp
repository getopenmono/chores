// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#ifndef battery_view_h
#define battery_view_h
#include <mono.h>

using namespace mono::ui;
using namespace mono::geo;

class BatteryView
:
  public View
{
  uint8_t lastBatteryPercent;
  uint8_t criticalAmount;
  int lastBatteryPixelLen;
  uint8_t dotSize;
  Color batteryCriticalColor;
  Color borderColor, backgroundColor;
  bool blinking, lastBlinkDrawed;
  mono::Timer blinker;
  void drawBounds ();
  void drawDot ();
  void drawBatteryLevel ();
  void checkForCriticalLevel ();
public:
  BatteryView (Rect const & rect);
  void setBatteryPercent (uint8_t percent);
  uint8_t batteryPercent () const;
  void setBatteryCritical (Color const & col);
  void setBorder (Color const & col);
  void setBackground (Color const & col);
  void setBlinkWhenCritical (bool blink);
  void setCriticalPercentAmount (uint8_t critical);
  // Inherited methods.
  void repaint();
};

#endif // battery_view_h
