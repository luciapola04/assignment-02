#ifndef __CONTAINER_HEALTH_CHECK_TASK__
#define __CONTAINER_HEALTH_CHECK_TASK__

#include "kernel/Task.h"
#include <WString.h>
#include <model/Context.h>

class AlarmTask: public Task {

public:
  AlarmTask(HWPlatform* pHw,Context* pContext, UserPanel* pUserPanel); 
  void tick();

private:  
  void setState(AlarmState state);
  long elapsedTimeInState();
  void log(const String& msg);
  
  bool checkAndSetJustEntered();
  
  AlarmState state;
  long stateTimestamp;
  bool justEntered;

  Context* pContext;
  HWPlatform* pHw;
  UserPanel* pUserPanel;

  float currentTemp;
  bool buttonReset;
};

#endif