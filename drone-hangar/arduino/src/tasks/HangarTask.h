#ifndef __HANGAR_TASK__
#define __HANGAR_TASK__

#include "kernel/Task.h"
#include "HWPlatform.h"
#include "Context.h"


class HangarTask: public Task {
public:
  HangarTask(Context* pContext);
  virtual void tick();

private:
  void setState(HangarState s);
  long elapsedTimeInState();
  bool checkAndSetJustEntered();

  Context* pContext;

  bool isClosing;
  long doorTimer;

  HangarState state;
  long stateTimestamp;
  bool justEntered;  
};

#endif