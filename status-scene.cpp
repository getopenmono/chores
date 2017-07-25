// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#include "status-scene.hpp"

StatusScene::StatusScene ()
:
  SceneController(Rect(0, 0, 176, 10)),
  battery(Rect(viewRect.X2()-23, 1, 18, 9)),
  chargingTimer(1000, false)
{
  addView(battery);
  battery.setBlinkWhenCritical(true);
  battery.setBatteryPercent(100);
  IApplicationContext * cnxt = IApplicationContext::Instance;
  if (cnxt != 0)
      cnxt->PowerManager->AppendToPowerAwareQueue(this);
  chargingTimer.setCallback(this, &StatusScene::updateBattery);
  setDismissCallback(this, &StatusScene::handleDismiss);
}

void StatusScene::updateBattery ()
{
  if (IApplicationContext::Instance->PowerManager->PowerSystem->IsUSBCharging())
    showCapacityRemaining();
  else
    showCharging();
}

void StatusScene::showCharging ()
{
  battery.setCriticalPercentAmount(20);
  battery.setBorder(View::StandardBorderColor);
  int percent = mono::power::MonoBattery::ReadPercentage();
  if (percent == 0)
    // It can take some time to read battery, and if so try again.
    onSystemWakeFromSleep();
  else
    battery.setBatteryPercent(percent);
}

void StatusScene::showCapacityRemaining ()
{
  battery.setCriticalPercentAmount(0);
  battery.setBorder(GreenColor);
  uint8_t newPercent = battery.batteryPercent() + 20;
  if (newPercent > 100)
    newPercent = 20;
  battery.setBatteryPercent(newPercent);
  if (!chargingTimer.Running())
    chargingTimer.start();
}

void StatusScene::handleDismiss ()
{
  hide();
}

void StatusScene::onSystemPowerOnReset ()
{
  onSystemWakeFromSleep();
}

void StatusScene::onSystemWakeFromSleep ()
{
  mono::Timer::callOnce(500, this, &StatusScene::updateBattery);
}
