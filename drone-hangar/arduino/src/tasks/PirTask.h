#ifndef __PIR_TASK__
#define __PIR_TASK__

#include "kernel/Task.h"
#include "devices/Sonar.h"
#include "model/Context.h"
#include <Arduino.h>


class PirTask: public Task {

public:
  PirTask(Pir* pPir, Context* pContext); 
  void tick();

private:
    PirState state;
    Pir* pPir;
    Context* pContext;
    
    
    long stateTimestamp;
    bool justEntered;

    void setState(PirState s);
    long elapsedTimeInState();
    bool checkAndSetJustEntered();
};

#endif