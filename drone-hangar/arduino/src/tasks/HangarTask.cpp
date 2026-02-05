#include "tasks/HangarTask.h"
#include "Arduino.h"
#include "kernel/Logger.h"
#include "config.h"

HangarTask::HangarTask(HWPlatform* pHW, Context* pContext): pHw(pHW) ,pContext(pContext) {
    setState(STARTUP);
}
  
void HangarTask::tick(){

    pContext->sync();

    if (pContext->getSystemState() == ALARM && state != ALARM_STATE) {
        setState(ALARM_STATE);
    }

    switch (state){   
        
        case STARTUP: {
            if (this->checkAndSetJustEntered()) {
                Logger.log("Dentro STARTUP");
                pContext->reset();
            }
            setState(IDLE);
            break;
        }

        case IDLE: {
            if(this->checkAndSetJustEntered()) {
                Logger.log("Dentro IDLE");
            }

            bool flightOk = !pContext->isInPreAlarm();

            if (pContext->isTakeOffRequest()) {
                Logger.log("Richiesta TAKE OFF");
                if (flightOk && pContext->isDroneInside()) {
                    setState(TAKE_OFF);
                } else {
                    Logger.log("ERRORE: Drone gia' fuori!");
                }
                pContext->setTakeOffRequest(false);
            } else if (pContext->isLandingRequest()) {
                pContext->setDronePresent(pHw->getPir()->isDetected() ? true : false);
                Logger.log("Richiesta LANDING");
                if (flightOk && !pContext->isDroneInside() && pContext->isDronePresent()) {
                    setState(LANDING);
                } else {
                    Logger.log("ERRORE: Drone dentro o non rilevato dal PIR!");
                }
                pContext->setLandingRequest(false);
            }
            break;
        }

        case TAKE_OFF: {
            if (this->checkAndSetJustEntered()) {
                Logger.log("Dentro TAKEOFF");
                pContext->setDoorCommand(CMD_OPEN);
            }

            if (pContext->getDoorCommand() == CMD_OPEN) {
                if (pContext->isDoorOpen()) {

                    float currentDist = pHw->getSonar()->getDistance();
                    
                    if (currentDist > D1) {
                        if (elapsedTimeInState() > (T_DOOR_MOVE + T1)) {
                            Logger.log("Drone Uscito. Chiudo...");
                            pContext->setDoorCommand(CMD_CLOSE);
                            pContext->setDroneInside(false);
                        }
                    } else {
                        this->stateTimestamp = millis();
                    }
                }
            } else if (pContext->getDoorCommand() == CMD_CLOSE) {
                if (pContext->isDoorClose()) {
                    Logger.log("Chiusura completata");
                    setState(IDLE);
                }
            }
            break;
        }

        case LANDING: {
            if (this->checkAndSetJustEntered()) {
                Logger.log("Dentro LANDING");
                pContext->setDoorCommand(CMD_OPEN);
            }

            if (pContext->getDoorCommand() == CMD_OPEN) {
                if (pContext->isDoorOpen()) {

                    float currentDist = pHw->getSonar()->getDistance();

                    if (currentDist < D2) {
                        if (elapsedTimeInState() > (T_DOOR_MOVE + T2)) {
                            Logger.log("Drone Entrato. Chiudo...");
                            pContext->setDoorCommand(CMD_CLOSE);
                            pContext->setDroneInside(true);
                        }
                    } else {
                        this->stateTimestamp = millis();
                    }
                }
            } else if (pContext->getDoorCommand() == CMD_CLOSE) {
                if (pContext->isDoorClose()) {
                    Logger.log("Chiusura completata");
                    setState(IDLE);
                }
            }
            break;
        }

        case ALARM_STATE: {
            if (this->checkAndSetJustEntered()) {
                Logger.log("ALLARME!!!");
                pHw->getL3()->switchOn();
                pHw->getL1()->switchOff();

                if(pContext->isDoorOpen()){
                    pContext->setDoorCommand(CMD_CLOSE);
                }
            }

            if (pContext->checkResetButtonAndReset()) {
                Logger.log("RESETTING ALLARME");
                pHw->getL3()->switchOff();
                pHw->getL1()->switchOn();
                pHw->getMotor()->off();
                setState(STARTUP);
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