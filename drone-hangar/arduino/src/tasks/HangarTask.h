#ifndef __HANGAR_TASK__
#define __HANGAR_TASK__

#include "kernel/Task.h"
#include "HWPlatform.h"
#include "Context.h"

class HangarTask: public Task {

public:
  HangarTask(HWPlatform* pHW, Context* pContext);
  virtual void tick();

private:
  enum HangarState {STARTUP, IDLE, TAKE_OFF, LANDING}; //da aggiornare
  void setState(HangarState s);
  long elapsedTimeInState();
  bool checkAndSetJustEntered();

  HWPlatform* pHW;
  Context* pContext;

  HangarState state;
  long stateTimestamp;
  bool justEntered;

  
};

#endif