// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#include "app_controller.h"
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

using mono::display::Color;
using mono::geo::Point;
using mono::geo::Rect;
using mono::IApplicationContext;
using mono::String;
using mono::ui::IconView;
using mono::ui::TextLabelView;

#define DEFAULT_ICONS []

#define ICONS_PER_MAIN_PAGE 3

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
  statusScene.show();
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
  // Preselect Dog, Plant & Vacuum Cleaner.
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
  statusScene.requestDismiss();
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
