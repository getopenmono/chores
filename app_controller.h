// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#ifndef app_controller_h
#define app_controller_h
#include <mono.h>
#include <vector>

using mono::DateTime;
using mono::display::MonoIcon;
using mono::TouchEvent;
using mono::TouchResponder;
using mono::ui::IconView;
using mono::ui::SceneController;
using mono::ui::TextLabelView;
using mono::String;

struct Chore
{
  MonoIcon const * icon;
  uint32_t unixLastDone;
  Chore (MonoIcon const *);
};

class AppController;

struct TimeUnit;

class MainScene
:
  public SceneController,
  public TouchResponder
{
  AppController * app;
  IconView icon1;
  TextLabelView counter1;
  TextLabelView unit1;
  IconView icon2;
  TextLabelView counter2;
  TextLabelView unit2;
  IconView icon3;
  TextLabelView counter3;
  TextLabelView unit3;
  TimeUnit calculateTimeSinceLastDone (uint32_t lastDone);
  void updateOneChore (TextLabelView & counter, TextLabelView & unit, TimeUnit const & tu);
  void handleShow (SceneController const &);
  void handleDismiss ();
public:
  MainScene (AppController *);
  void updateTime (Chore const * c1, Chore const * c2, Chore const * c3);
  void setChores (Chore const * c1, Chore const * c2, Chore const * c3);
  void RespondTouchEnd (TouchEvent &);
};

class SelectScene
:
  public SceneController,
  public TouchResponder
{
  AppController * app;
  IconView icon1;
  IconView icon2;
  IconView icon3;
  IconView icon4;
  IconView icon5;
  IconView icon6;
  IconView left;
  IconView right;
  void handleShow (SceneController const &);
  void handleDismiss ();
public:
  SelectScene (AppController *);
  void initializeIcons (std::vector<Chore const *> const &);
  void showPage (std::vector<Chore const *> const &);
  void RespondTouchEnd (TouchEvent &);
};

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
