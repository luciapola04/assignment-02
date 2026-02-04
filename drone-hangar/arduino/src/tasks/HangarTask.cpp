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
            if (this->checkAndSetJustEntered()) {
                Logger.log("Dentro STARTUP");
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
            if(this->checkAndSetJustEntered()) {
                Logger.log("Dentro IDLE");
            }

            if (pContext->isStarted() && pContext->getSystemState() == NORMAL) {
                if (pContext->isDroneInside()) {
                    setState(TAKE_OFF);
                } else if(pContext->isDronePresent()) {
                    setState(LANDING);
                }
            }
            break;
        }

        case TAKE_OFF: {
            if (this->checkAndSetJustEntered()) {
                Logger.log("Dentro TAKEOFF");
                pHW->getMotor()->setPosition(MOTOR_OPEN_POS);
                pHW->getLcd()->clear();
                pHW->getLcd()->print("TAKE OFF");

                pContext->setDoor(true);
            }
            
            if (pContext->getDroneDistance() > D1) {
                if (this->elapsedTimeInState() > T1) {
                    pHW->getMotor()->setPosition(MOTOR_CLOSE_POS);
                    pHW->getLcd()->clear();
                    pHW->getLcd()->print("DRONE OUT");
                    
                    pContext->setDroneInside(false);
                    pContext->setDoor(false);
                    pContext->setStopped();
                    setState(IDLE);
                }
            } else {
                this->stateTimestamp = millis();
            }
            break;
        }

        case LANDING: {
            if (this->checkAndSetJustEntered()) {
                Logger.log("Dentro LANDING");
                pHW->getMotor()->setPosition(MOTOR_OPEN_POS);
                pHW->getLcd()->clear();
                pHW->getLcd()->print("LANDING");

                pContext->setDoor(true);
            }

            if (pContext->getDroneDistance() < D2) {
                if (this->elapsedTimeInState() > T2) {
                    pHW->getMotor()->setPosition(MOTOR_CLOSE_POS);
                    pHW->getLcd()->clear();
                    pHW->getLcd()->print("DRONE INSIDE");
                    
                    pContext->setDroneInside(true);
                    pContext->setDoor(false);
                    pContext->setStopped();
                    setState(IDLE);
                } 
            } else {
                this->stateTimestamp = millis();
            }
            break;
        }
    }
}

// --- Funzioni di supporto FSM ---

void HangarTask::setState(HangarState s){
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