// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#include "app_controller.h"
#include "icons/arrow-left.h"
#include "icons/arrow-right.h"
#include "icons/bin.h"
#include "icons/bird.h"
#include "icons/boot.h"
#include "icons/car.h"
#include "icons/cat.h"
#include "icons/cloth.h"
#include "icons/coffee.h"
#include "icons/dog.h"
#include "icons/dumbbell.h"
#include "icons/faucet.h"
#include "icons/fish.h"
#include "icons/grill.h"
#include "icons/knife.h"
#include "icons/oven.h"
#include "icons/pipes.h"
#include "icons/plant.h"
#include "icons/razor.h"
#include "icons/scissors.h"
#include "icons/spray.h"
#include "icons/toilet.h"
#include "icons/towel.h"
#include "icons/tweezers.h"
#include "icons/vacuum-cleaner.h"
#include <algorithm>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

using mono::display::Color;
using mono::geo::Point;
using mono::geo::Rect;
using mono::IApplicationContext;
using mono::String;
using mono::ui::IconView;
using mono::ui::TextLabelView;

#define DEFAULT_ICONS []

#define BACKGROUND BlackColor
#define ICON_FOREGROUND AsbestosColor
#define COUNTER_FOREGROUND CloudsColor
#define UNIT_FOREGROUND AsbestosColor
#define ICONS_PER_MAIN_PAGE 3
#define ICONS_PER_SELECTION_PAGE 6

Chore::Chore (MonoIcon const * icon_)
:
  icon(icon_),
  unixLastDone(0)
{}

struct TimeUnit
{
  char const * time;
  char const * unit;
};

AppController::AppController ()
:
  mainScene(this),
  selectScene(this),
  sleeper(2 * 60 * 1000),
  tick(1000)
{
  sleeper.setCallback(IApplicationContext::EnterSleepMode);
  tick.setCallback(this, &AppController::updateTime);
  setupDefaults();
  setupMainScene();
  setupSelectScene();
}

void AppController::debugLine (String msg)
{
  printf(String::Format("%s\r\n",msg())());
}

void AppController::monoWakeFromReset ()
{
  monoWakeFromSleep();
}

void AppController::monoWillGotoSleep ()
{
  debugLine("Sleeping...");
}

void AppController::monoWakeFromSleep ()
{
  selectScene.requestDismiss();
  mainScene.show();
  mainScene.scheduleRepaint();
  updateTime();
  sleeper.start();
  tick.start();
}

void AppController::setupDefaults ()
{
  MonoIcon const * icons[] = {
    &mono::display::bin,
    &mono::display::bird,
    &mono::display::boot,
    &mono::display::car,
    &mono::display::cat,
    &mono::display::cloth,
    &mono::display::coffee,
    &mono::display::dog,
    &mono::display::dumbbell,
    &mono::display::faucet,
    &mono::display::fish,
    &mono::display::grill,
    &mono::display::knife,
    &mono::display::oven,
    &mono::display::pipes,
    &mono::display::plant,
    &mono::display::razor,
    &mono::display::scissors,
    &mono::display::spray,
    &mono::display::toilet,
    &mono::display::towel,
    &mono::display::tweezers,
    &mono::display::vacuumCleaner,
    0
  };
  for (size_t i = 0; icons[i] != 0; ++i)
  {
    chores.push_back(Chore(icons[i]));
  }
  selectedChores.push_back(7);
  selectedChores.push_back(15);
  selectedChores.push_back(22);
  selectionToReplace = 0;
}

void AppController::setupMainScene ()
{
  mainScene.setChores
  (
    &chores[selectedChores[0]],
    &chores[selectedChores[1]],
    &chores[selectedChores[2]]
  );
}

void AppController::setupSelectScene ()
{
  std::vector<Chore const *> selection;
  for (size_t i = 0; i < ICONS_PER_SELECTION_PAGE; ++i)
    selection.push_back(&chores[i]);
  selectScene.initializeIcons(selection);
}

void AppController::updateTime ()
{
  mainScene.updateTime
  (
    &chores[selectedChores[0]],
    &chores[selectedChores[1]],
    &chores[selectedChores[2]]
  );
}

void AppController::timeReset (size_t choreIndex)
{
  sleeper.start();
  chores[selectedChores[choreIndex]].unixLastDone = DateTime::now().toUnixTime();
  updateTime();
}

void AppController::changeChore (size_t selectedChoreIndex)
{
  if (selectedChoreIndex >= ICONS_PER_MAIN_PAGE)
  {
    debugLine(String::Format("Invalid icon %d", selectedChoreIndex));
    return;
  }
  sleeper.start();
  mainScene.requestDismiss();
  selectScene.show();
  selectScene.scheduleRepaint();
  tick.stop();
  selectionToReplace = selectedChoreIndex;
  selectIconsAfterAndNotIncludingSelected(selectedChores[selectionToReplace]);
}

void AppController::selectIconsAfterAndNotIncludingSelected (size_t choreIndex)
{
  selection.clear();
  size_t candidate = choreIndex + 1;
  while (selection.size() < ICONS_PER_SELECTION_PAGE)
  {
    if (candidate >= chores.size())
      candidate = 0;
    if (choreSelected(candidate))
    {
      ++candidate;
      continue;
    }
    selection.push_back(&chores[candidate]);
    ++candidate;
  }
  selectScene.showPage(selection);
}

bool AppController::choreSelected (size_t choreIndex) const
{
  return
    selectedChores.end() !=
    std::find(selectedChores.begin(), selectedChores.end(), choreIndex);
}

void AppController::replaceChoreWithSelection (size_t index)
{
  if (index >= ICONS_PER_SELECTION_PAGE)
  {
    debugLine(String::Format("Invalid selection %d", index));
    return;
  }
  Chore const * chore = selection[index];
  for (size_t i = 0; i < chores.size(); ++i)
    if (chore == const_cast<Chore const *>(&chores[i]))
    {
      selectedChores[selectionToReplace] = i;
      updateAndGoToMainScene();
      return;
    }
  debugLine(String::Format("Selection %d not found", index));
}

void AppController::updateAndGoToMainScene ()
{
  setupMainScene();
  monoWakeFromSleep();
}

void AppController::previousSelectionPage ()
{
  Chore const * firstChore = selection[0];
  size_t candidate = 0;
  for (; candidate < chores.size(); ++candidate)
    if (firstChore == const_cast<Chore const *>(&chores[candidate]))
      break;
  size_t missing = ICONS_PER_SELECTION_PAGE + 1;
  while (missing > 0)
  {
    if (candidate == 0)
      candidate = chores.size() - 1;
    else
      --candidate;
    if (choreSelected(candidate))
      continue;
    --missing;
  }
  selectIconsAfterAndNotIncludingSelected(candidate);
  selectScene.scheduleRepaint();
}

void AppController::nextSelectionPage ()
{
  Chore const * lastChore = selection[ICONS_PER_SELECTION_PAGE-1];
  for (size_t i = 0; i < chores.size(); ++i)
    if (lastChore == const_cast<Chore const *>(&chores[i]))
    {
      selectIconsAfterAndNotIncludingSelected(i);
      selectScene.scheduleRepaint();
      return;
    }
}

MainScene::MainScene (AppController * application)
:
  SceneController(Rect(0, 0, 176, 220)),
  app(application),
  counter1(Rect(75, 5, 70, 64), "?"),
  unit1(Rect(152, 15, 34, 55), "?"),
  counter2(Rect(75, 78, 70, 64), "?"),
  unit2(Rect(152, 88, 34, 55), "?"),
  counter3(Rect(75, 151, 70, 64), "?"),
  unit3(Rect(152, 161, 34, 55), "?")
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
  Activate();
}

void MainScene::handleDismiss ()
{
  Deactivate();
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
  icon1 = IconView(Point(10, 5), *c1->icon);
  icon1.setBackground(BACKGROUND);
  icon1.setForeground(ICON_FOREGROUND);
  TimeUnit tu = calculateTimeSinceLastDone(c1->unixLastDone);
  counter1.setText(tu.time);
  unit1.setText(tu.unit);

  icon2 = IconView(Point(10, 78), *c2->icon);
  icon2.setBackground(BACKGROUND);
  icon2.setForeground(ICON_FOREGROUND);
  tu = calculateTimeSinceLastDone(c2->unixLastDone);
  counter2.setText(tu.time);
  unit2.setText(tu.unit);

  icon3 = IconView(Point(10, 151), *c3->icon);
  icon3.setBackground(BACKGROUND);
  icon3.setForeground(ICON_FOREGROUND);
  tu = calculateTimeSinceLastDone(c3->unixLastDone);
  counter3.setText(tu.time);
  unit3.setText(tu.unit);
}

void MainScene::RespondTouchEnd (TouchEvent & touch)
{
  int x = touch.TouchController->ToScreenCoordsX(touch.Position.X(), 176);
  int y = touch.TouchController->ToScreenCoordsX(touch.Position.Y(), 220);
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

SelectScene::SelectScene (AppController * application)
:
  SceneController(Rect(0, 0, 176, 220)),
  app(application)
{
  left = IconView(Point(0, 192), arrowLeft);
  left.setBackground(BACKGROUND);
  left.setForeground(ICON_FOREGROUND);
  addView(left);
  right = IconView(Point(88, 192), arrowRight);
  right.setBackground(BACKGROUND);
  right.setForeground(ICON_FOREGROUND);
  addView(right);
  setShowCallback(this, &SelectScene::handleShow);
  setDismissCallback(this, &SelectScene::handleDismiss);
}

void SelectScene::initializeIcons (std::vector<Chore const *> const & selection)
{
  if (selection.size() != ICONS_PER_SELECTION_PAGE)
  {
    app->debugLine(String::Format("Only %d selections", selection.size()));
    return;
  }
  icon1 = IconView(Point(16, 0), *selection[0]->icon);
  icon1.setBackground(BACKGROUND);
  icon1.setForeground(ICON_FOREGROUND);
  icon2 = IconView(Point(16, 64), *selection[1]->icon);
  icon2.setBackground(BACKGROUND);
  icon2.setForeground(ICON_FOREGROUND);
  icon3 = IconView(Point(16, 128), *selection[2]->icon);
  icon3.setBackground(BACKGROUND);
  icon3.setForeground(ICON_FOREGROUND);
  icon4 = IconView(Point(96, 0), *selection[3]->icon);
  icon4.setBackground(BACKGROUND);
  icon4.setForeground(ICON_FOREGROUND);
  icon5 = IconView(Point(96, 64), *selection[4]->icon);
  icon5.setBackground(BACKGROUND);
  icon5.setForeground(ICON_FOREGROUND);
  icon6 = IconView(Point(96, 128), *selection[5]->icon);
  icon6.setBackground(BACKGROUND);
  icon6.setForeground(ICON_FOREGROUND);
  addView(icon1);
  addView(icon2);
  addView(icon3);
  addView(icon4);
  addView(icon5);
  addView(icon6);
}

void SelectScene::RespondTouchEnd (TouchEvent & touch)
{
  int x = touch.TouchController->ToScreenCoordsX(touch.Position.X(), 176);
  int y = touch.TouchController->ToScreenCoordsX(touch.Position.Y(), 220);
  if (x < 88)
  {
    if (y < 64)
      app->replaceChoreWithSelection(0);
    else if (y < 128)
      app->replaceChoreWithSelection(1);
    else if (y < 192)
      app->replaceChoreWithSelection(2);
    else
      app->previousSelectionPage();
  }
  else
  {
    if (y < 64)
      app->replaceChoreWithSelection(3);
    else if (y < 128)
      app->replaceChoreWithSelection(4);
    else if (y < 192)
      app->replaceChoreWithSelection(5);
    else
      app->nextSelectionPage();
  }
}

void SelectScene::showPage (std::vector<Chore const *> const & selection)
{
  if (selection.size() != ICONS_PER_SELECTION_PAGE)
  {
    app->debugLine(String::Format("Only %d selections", selection.size()));
    return;
  }
  icon1.setIcon(selection[0]->icon);
  icon2.setIcon(selection[1]->icon);
  icon3.setIcon(selection[2]->icon);
  icon4.setIcon(selection[3]->icon);
  icon5.setIcon(selection[4]->icon);
  icon6.setIcon(selection[5]->icon);
}

void SelectScene::handleShow (SceneController const &)
{
  Activate();
}

void SelectScene::handleDismiss ()
{
  Deactivate();
  hide();
}
