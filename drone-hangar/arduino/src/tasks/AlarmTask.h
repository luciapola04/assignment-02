#ifndef __CONTAINER_HEALTH_CHECK_TASK__
#define __CONTAINER_HEALTH_CHECK_TASK__

#include "kernel/Task.h"
#include <WString.h>
#include <Context.h>

class AlarmTask: public Task {

public:
  AlarmTask(Context* pContext); 
  void tick();

private:  
  enum State { 
      NORMAL, 
      CHECKING_PRE_ALARM, // Sta contando T3
      PRE_ALARM,          // Temp1 superata per > T3
      CHECKING_ALARM,     // Sta contando T4
      ALARM               // Temp2 superata per > T4
  };
  void setState(State state);
  long elapsedTimeInState();
  void log(const String& msg);
  
  bool checkAndSetJustEntered();
  
  State state;
  long stateTimestamp;
  bool justEntered;

  Context* pContext;
};

#endif