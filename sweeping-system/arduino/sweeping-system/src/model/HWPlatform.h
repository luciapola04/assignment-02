#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "config.h"
#include "devices/Button.h"
#include "devices/Lcd.h"
#include "devices/Led.h"
#include "devices/ServoMotor.h"
#include "devices/Sonar.h"
#include "devices/TempSensor.h"
#include "devices/Pir.h"

class HWPlatform {

public:
  HWPlatform();
  void init();

  Button* getButton();
  Lcd* getLcd();
  Led* getL1();
  Led* getL2();
  Led* getL3();
  ServoMotor* getMotor();
  Sonar* getSonar();
  TempSensor* getTempSensor();
  Pir* getPir();

private:
  Button* resetButton;
  Lcd* lcd;
  Led* L1;
  Led* L2;
  Led* L3;
  ServoMotor* servoMotor;
  Sonar* sonar;
  TempSensor* tempSensor;
  Pir* pir;
};

#endif