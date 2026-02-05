#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "ButtonImpl.h"
#include "config.h"
#include <LiquidCrystal_I2C.h>
#include "Led.h"
#include "Pir.h"
#include "ServoMotorImpl.h"
#include "Sonar.h"
#include "TempSensorLM35.h" 


class HWPlatform {

public:
  HWPlatform();
  void init();
  void test();

  Button* getButton();
  LiquidCrystal_I2C* getLcd();
  Led* getL1();
  Led* getL2();
  Led* getL3();
  ServoMotor* getMotor();
  Sonar* getSonar();
  TempSensor* getTempSensor();
  Pir* getPir();

private:
  Button* resetButton;
  LiquidCrystal_I2C* lcd;
  Led* L1;
  Led* L2;
  Led* L3;
  ServoMotor* servoMotor;
  Sonar* sonar;
  TempSensor* tempSensor;
  Pir* pir;
};

#endif