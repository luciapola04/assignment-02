#include "tasks/HangarTask.h"
#include "Arduino.h"
#include "kernel/Logger.h"
#include "config.h"

HangarTask::HangarTask(HWPlatform* pHW, Context* pContext): 
    pHW(pHW), pContext(pContext) {
    setState(STARTUP);
}
  
void HangarTask::tick(){
    switch (state){   
        
        case STARTUP: {
            if (this->checkAndSetJustEntered()){
                pHW->getL1()->switchOn();
                pHW->getL2()->switchOff();
                pHW->getL3()->switchOff();
                pHW->getMotor()->on();
                pHW->getMotor()->setPosition(MOTOR_CLOSE_POS);
                pHW->getLcd()->clear();
                pHW->getLcd()->print("DRONE INSIDE");
            }
            setState(IDLE);
            break;
        }

        case IDLE: {
            break;
        }
    }
}

// --- Funzioni di supporto FSM ---

void HangarTask::setState(int s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long HangarTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool HangarTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}