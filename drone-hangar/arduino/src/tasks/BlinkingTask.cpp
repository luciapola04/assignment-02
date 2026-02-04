#include "tasks/BlinkingTask.h"
#include "HangarTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

BlinkingTask::BlinkingTask(Led* pLed, Context* pContext): 
    pContext(pContext), pLed(pLed){
    setState(IDLE);
}
  
void BlinkingTask::tick(){
    
    bool blink = (pContext->isDoorOpen() || pContext->isInPreAlarm());
    switch (state){   
    case IDLE: {
        if (this->checkAndSetJustEntered()){
            pLed->switchOff();
            Logger.log(F("[BT] IDLE"));
        }
        if(blink){
            setState(ON);
        }
        break;
    }
    case OFF: {
        if (this->checkAndSetJustEntered()){
            pLed->switchOff();
            Logger.log(F("[BT] OFF"));
        }
        if(elapsedTimeInState() > BLINK_PERIOD){
            setState(ON);
        }
        if(!blink){
            setState(IDLE);
        }
        break;
    }
    case ON: {
        if (this->checkAndSetJustEntered()){
            pLed->switchOn();
            Logger.log(F("[BT] ON"));
        }
        if(elapsedTimeInState() > BLINK_PERIOD){
            setState(OFF);
        }
        if(!blink){
            setState(IDLE);
        }
        break;
    }
    }
}


void BlinkingTask::setState(State s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long BlinkingTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool BlinkingTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}
