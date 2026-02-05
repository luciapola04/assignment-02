#ifndef __HANGAR_TASK__
#define __HANGAR_TASK__

#include "kernel/Task.h"
#include "model/HWPlatform.h"
#include "model/Context.h"


class HangarTask: public Task {
public:
  HangarTask(HWPlatform* pHw,Context* pContext);
  virtual void tick();

private:
  void setState(HangarState s);
  long elapsedTimeInState();
  bool checkAndSetJustEntered();

  Context* pContext;
  HWPlatform* pHw;

  bool isClosing;
  long doorTimer;

  HangarState state;
  long stateTimestamp;
  bool justEntered;  
};

#endif