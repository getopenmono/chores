// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#include "battery-view.hpp"

BatteryView::BatteryView (const Rect &rct)
:
	View(rct),
  blinker(650, false)
{
  lastBatteryPercent = 0;
  criticalAmount = 20;
  dotSize = 1;
  lastBatteryPixelLen = 0;
  lastBatteryPercent = 0;
  batteryCriticalColor = RedColor;
  borderColor = StandardBorderColor;
  backgroundColor = StandardBackgroundColor;
  blinking = false;
  lastBlinkDrawed = false;
  blinker.setCallback<BatteryView>(this, &BatteryView::scheduleRepaint);
}

void BatteryView::setBatteryPercent (uint8_t percent)
{
  if (lastBatteryPercent == percent)
    return;
  lastBatteryPercent = percent;
  int fullLength = viewRect.Width() - dotSize - 4;
  lastBatteryPixelLen = lastBatteryPercent * fullLength / 100;
  scheduleRepaint();
}

uint8_t BatteryView::batteryPercent () const
{
  return lastBatteryPercent;
}

void BatteryView::setBatteryCritical (Color const & col)
{
  batteryCriticalColor = col;
}

void BatteryView::setBorder (Color const & col)
{
  borderColor = col;
}

void BatteryView::setBackground (Color const & col)
{
  backgroundColor = col;
}

void BatteryView::setBlinkWhenCritical (bool blink)
{
  blinking = blink;
  if (lastBlinkDrawed <= criticalAmount)
    scheduleRepaint();
}

void BatteryView::setCriticalPercentAmount (uint8_t critical)
{
  criticalAmount = critical;
}

void BatteryView::repaint()
{
  painter.setForegroundColor(borderColor);
  drawBounds();
  drawDot();
  checkForCriticalLevel();
	drawBatteryLevel();
  if (blinking && lastBatteryPercent <= criticalAmount)
    lastBlinkDrawed = !lastBlinkDrawed;
}

void BatteryView::drawBounds ()
{
  Rect bounds(viewRect.X(), viewRect.Y(), viewRect.Width() - dotSize, viewRect.Height());
  painter.drawFillRect(bounds, true);
  painter.drawRect(bounds);
}

void BatteryView::drawDot ()
{
  Point d1(viewRect.X2() - dotSize, viewRect.Y() + viewRect.Height()/4);
  Point d2(viewRect.X2() - dotSize, viewRect.Y2() - viewRect.Height()/4);
  painter.drawLine(d1, d2);
}

void BatteryView::drawBatteryLevel ()
{
  Rect bounds(viewRect.X(), viewRect.Y(), viewRect.Width() - dotSize, viewRect.Height());
  Rect bat(bounds.X()+2, bounds.Y()+2, lastBatteryPixelLen, bounds.Height() - 4);
  painter.drawFillRect(bat);
}

void BatteryView::checkForCriticalLevel ()
{
  if (lastBatteryPercent <= criticalAmount)
  {
    if (blinking)
      blinker.start();
    if (blinking && lastBlinkDrawed)
      painter.setForegroundColor(backgroundColor);
    else
      painter.setForegroundColor(batteryCriticalColor);
  }
  else
    blinker.stop();
}
