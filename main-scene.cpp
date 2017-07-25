// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#include "main-scene.hpp"
#include "chore.hpp"
#include "app_controller.h"
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

using mono::geo::Rect;
using mono::geo::Point;

MainScene::MainScene (AppController * application)
:
  SceneController(Rect(0, 20, 176, 220)),
  app(application),
  counter1(Rect(75, 20, 70, 64), "?"),
  unit1(Rect(152, 30, 34, 55), "?"),
  counter2(Rect(75, 88, 70, 64), "?"),
  unit2(Rect(152, 98, 34, 55), "?"),
  counter3(Rect(75, 156, 70, 64), "?"),
  unit3(Rect(152, 166, 34, 55), "?")
{
  counter1.setFont(FreeMonoBold24pt7b);
  counter1.setText(COUNTER_FOREGROUND);
  counter1.setAlignment(TextLabelView::ALIGN_RIGHT);

  unit1.setFont(FreeSans12pt7b);
  unit1.setText(UNIT_FOREGROUND);

  counter2.setFont(FreeMonoBold24pt7b);
  counter2.setText(COUNTER_FOREGROUND);
  counter2.setAlignment(TextLabelView::ALIGN_RIGHT);

  unit2.setFont(FreeSans12pt7b);
  unit2.setText(UNIT_FOREGROUND);

  counter3.setFont(FreeMonoBold24pt7b);
  counter3.setText(COUNTER_FOREGROUND);
  counter3.setAlignment(TextLabelView::ALIGN_RIGHT);

  unit3.setFont(FreeSans12pt7b);
  unit3.setText(UNIT_FOREGROUND);

  addView(icon1);
  addView(counter1);
  addView(unit1);
  addView(icon2);
  addView(counter2);
  addView(unit2);
  addView(icon3);
  addView(counter3);
  addView(unit3);

  setShowCallback(this, &MainScene::handleShow);
  setDismissCallback(this, &MainScene::handleDismiss);
}

void MainScene::handleShow (SceneController const &)
{
  activate();
}

void MainScene::handleDismiss ()
{
  deactivate();
  hide();
}

void MainScene::updateTime (Chore const * c1, Chore const * c2, Chore const * c3)
{
  updateOneChore(counter1, unit1, calculateTimeSinceLastDone(c1->unixLastDone));
  updateOneChore(counter2, unit2, calculateTimeSinceLastDone(c2->unixLastDone));
  updateOneChore(counter3, unit3, calculateTimeSinceLastDone(c3->unixLastDone));
}

void MainScene::updateOneChore (TextLabelView & counter, TextLabelView & unit, TimeUnit const & tu)
{
  if (strcmp(tu.unit, "-") == 0)
  {
    counter.setText(tu.unit);
    unit.setText("");
  }
  else
  {
    counter.setText(tu.time);
    unit.setText(tu.unit);
  }
}

void MainScene::setChores (Chore const * c1, Chore const * c2, Chore const * c3)
{
  icon1 = IconView(Point(10, 20), *c1->icon);
  icon1.setBackground(BACKGROUND);
  icon1.setForeground(ICON_FOREGROUND);
  TimeUnit tu = calculateTimeSinceLastDone(c1->unixLastDone);
  counter1.setText(tu.time);
  unit1.setText(tu.unit);

  icon2 = IconView(Point(10, 88), *c2->icon);
  icon2.setBackground(BACKGROUND);
  icon2.setForeground(ICON_FOREGROUND);
  tu = calculateTimeSinceLastDone(c2->unixLastDone);
  counter2.setText(tu.time);
  unit2.setText(tu.unit);

  icon3 = IconView(Point(10, 156), *c3->icon);
  icon3.setBackground(BACKGROUND);
  icon3.setForeground(ICON_FOREGROUND);
  tu = calculateTimeSinceLastDone(c3->unixLastDone);
  counter3.setText(tu.time);
  unit3.setText(tu.unit);
}

void MainScene::respondTouchEnd (TouchEvent & touch)
{
  int x = touch.TouchController->toScreenCoordsX(touch.Position.X(), 176);
  int y = touch.TouchController->toScreenCoordsX(touch.Position.Y(), 220);
  if (y < 73)
  {
    // First row.
    if (x < 69)
      app->changeChore(0);
    else
      app->timeReset(0);
  }
  else if (y < 146)
  {
    // Second row.
    if (x < 69)
      app->changeChore(1);
    else
      app->timeReset(1);
  }
  else
  {
    // Third row.
    if (x < 69)
      app->changeChore(2);
    else
      app->timeReset(2);
  }
}

char const * number [] = {
  "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
  "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
  "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
  "31", "32", "33", "34", "35", "36", "37", "38", "39", "40",
  "41", "42", "43", "44", "45", "46", "47", "48", "49", "50",
  "51", "52", "53", "54", "55", "56", "57", "58", "59", "60"
};

TimeUnit MainScene::calculateTimeSinceLastDone (uint32_t lastDone)
{
  if (lastDone == 0)
  {
    TimeUnit tu = { 0, "-" };
    return tu;
  }
  uint32_t now = DateTime::now().toUnixTime();
  uint32_t diff = now - lastDone;
  // Seconds?
  if (diff < 60)
  {
    TimeUnit tu = { number[diff], "s" };
    return tu;
  }
  diff /= 60;
  // Minutes?
  if (diff < 60)
  {
    TimeUnit tu = { number[diff], "m" };
    return tu;
  }
  diff /= 60;
  // Hours?
  if (diff < 24)
  {
    TimeUnit tu = { number[diff], "h" };
    return tu;
  }
  diff /= 24;
  // Days?
  if (diff < 10)
  {
    TimeUnit tu = { number[diff], "d" };
    return tu;
  }
  diff /= 7;
  // Weeks?
  if (diff < 9)
  {
    TimeUnit tu = { number[diff], "w" };
    return tu;
  }
  diff /= 4;
  // Months!
  TimeUnit tu = { number[diff], "M" };
  return tu;
}
