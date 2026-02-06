#include "tasks/BlinkingTask.h"
#include <Arduino.h>
#include "config.h"

BlinkingTask::BlinkingTask(Led* pLed, Context* pContext): 
     pLed(pLed),pContext(pContext){
    setState(IDLE);
}
  
void BlinkingTask::tick(){
    
    bool blink = (pContext->isInLanding() || pContext->isInTakeOff());
    
    switch (state){   
    case IDLE: {
        if (this->checkAndSetJustEntered()){
            pLed->switchOff();
        }
        if(blink){
            setState(ON);
        }
        break;
    }
    case OFF: {
        if (this->checkAndSetJustEntered()){
            pLed->switchOff();
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
