#ifndef __SONAR_TASK__
#define __SONAR_TASK__

#include "kernel/Task.h"
#include "devices/Sonar.h"
#include "model/Context.h"
#include <Arduino.h>


class SonarTask: public Task {

public:
  SonarTask(Sonar* pSonar, Context* pContext); 
  void tick();

private:
    SonarState state;
    Sonar* pSonar;
    Context* pContext;
    
    
    long stateTimestamp;
    bool justEntered;

    void setState(SonarState s);
    long elapsedTimeInState();
    bool checkAndSetJustEntered();
};

#endif