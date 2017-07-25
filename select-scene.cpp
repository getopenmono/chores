// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#include "select-scene.hpp"
#include "app_controller.h"
#include "icons/arrow-left.h"
#include "icons/arrow-right.h"

using mono::geo::Point;
using mono::geo::Rect;

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

void SelectScene::respondTouchEnd (TouchEvent & touch)
{
  int x = touch.TouchController->toScreenCoordsX(touch.Position.X(), 176);
  int y = touch.TouchController->toScreenCoordsX(touch.Position.Y(), 220);
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
  activate();
}

void SelectScene::handleDismiss ()
{
  deactivate();
  hide();
}
