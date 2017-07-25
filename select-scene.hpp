// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#ifndef select_scene_h
#define select_scene_h
#include <mono.h>
#include <vector>
#include "chore.hpp"

using mono::ui::SceneController;
using mono::ui::IconView;

class AppController;

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
  void respondTouchEnd (TouchEvent &);
};

#endif // select_scene_h
