// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#ifndef status_scene_h
#define status_scene_h
#include <mono.h>
#include "battery-view.hpp"

using namespace mono::ui;
using namespace mono::geo;

class StatusScene
:
  public SceneController,
  public mono::power::IPowerAware
{
  BatteryView battery;
  mono::Timer chargingTimer;
	void showCharging ();
	void showCapacityRemaining ();
  // Inherited methods.
  void handleDismiss ();
public:
  StatusScene ();
  void updateBattery();
  // Inherited methods.
  void onSystemPowerOnReset();
  void onSystemWakeFromSleep();
};

#endif // status_scene_h
