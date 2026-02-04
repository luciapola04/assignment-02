#ifndef __HANGAR_TASK__
#define __HANGAR_TASK__

#include "kernel/Task.h"
#include "HWPlatform.h"
#include "Context.h"

enum HangarState {STARTUP, IDLE, TAKE_OFF, LANDING, ALARM};

class HangarTask: public Task {
public:
  HangarTask(DroneHangar* hangar, Context* pContext);
  virtual void tick();

private:
  void setState(HangarState s);
  long elapsedTimeInState();
  bool checkAndSetJustEntered();

  DroneHangar* hangar;
  Context* pContext;

  HangarState state;
  long stateTimestamp;
  bool justEntered;  
};

#endif