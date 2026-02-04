#ifndef __WASTE_CONTAINER__
#define __WASTE_CONTAINER__

#include "config.h"
#include "model/HWPlatform.h"

class DroneHangar {

public:
  DroneHangar(HWPlatform* hw);
  void init();

  bool isAvailable();
  bool isInMaintenance();
  void reset();
  void setMaintenance();
   
  int getWasteLevelPercentage();
  float getCurrentTemperature();

  void startOpeningDoor();
  void stopOpeningDoor();
  void startClosingDoor();
  void stopClosingDoor();

  void setPreAlarm(bool active);
  void setAlarm(bool active);
  bool isPreAlarm();
  bool isAlarm();
  bool checkResetButton();

  void sync();

private:
  bool sensorsCanBeUsed();

  HWPlatform* pHW;

  float currentTemp;
  bool isAvail;
  float lastDistance;
  float lastTemperature;
  bool preAlarmState;
  bool alarmState;
};

#endif
