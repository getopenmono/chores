// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#ifndef main_scene_h
#define main_scene_h
#include <mono.h>

using mono::ui::IconView;
using mono::ui::SceneController;
using mono::ui::TextLabelView;

struct TimeUnit
{
  char const * time;
  char const * unit;
};

class Chore;
class AppController;

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
  void respondTouchEnd (TouchEvent &);
};

#endif // main_scene_h
