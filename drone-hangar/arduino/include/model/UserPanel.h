#ifndef __USER_PANEL__
#define __USER_PANEL__

#include "config.h"
#include "HWPlatform.h"
#include <LiquidCrystal_I2C.h>

class UserPanel {

public:
  UserPanel(HWPlatform* pHW);

  void init();

  void turnOffDisplay();
  void turnOnDisplay();

  void displayDroneInside();
  void displayTakeOff();
  void displayDroneOut();
  void displayLanding();
  void displayAlarm();
  
 private:
  LiquidCrystal_I2C* pLcd;
};

#endif