// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#ifndef app_controller_h
#define app_controller_h
#include <mono.h>
#include <vector>
#include "chore.hpp"
#include "main-scene.hpp"
#include "select-scene.hpp"

#define BACKGROUND BlackColor
#define ICON_FOREGROUND AsbestosColor
#define ICONS_PER_SELECTION_PAGE 6
#define COUNTER_FOREGROUND CloudsColor
#define UNIT_FOREGROUND AsbestosColor

class AppController
:
  public mono::IApplication
{
  std::vector<Chore> chores;
  // Index into chores.
  std::vector<size_t> selectedChores;
  // Index into selectedChores;
  size_t selectionToReplace;
  std::vector<Chore const *> selection;

  MainScene mainScene;
  SelectScene selectScene;

  mono::Timer sleeper;
  mono::Timer tick;
  void setupDefaults ();
  void setupMainScene ();
  void setupSelectScene ();
  void updateTime ();
  bool choreSelected (size_t choreIndex) const;
  void selectIconsAfterAndNotIncludingSelected (size_t choreIndex);
  void updateAndGoToMainScene ();
public:
  AppController ();
  void timeReset (size_t choreIndex);
  void changeChore (size_t selectedChoreIndex);
  void replaceChoreWithSelection (size_t index);
  void previousSelectionPage ();
  void nextSelectionPage ();
  void debugLine (mono::String msg);
  void monoWakeFromReset ();
  void monoWillGotoSleep ();
  void monoWakeFromSleep ();
};

#endif // app_controller_h
