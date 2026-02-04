#include "tasks/HangarTask.h"
#include "Arduino.h"
#include "kernel/Logger.h"
#include "config.h"
#include "DroneHangar.h"

HangarTask::HangarTask(DroneHangar* hangar, Context* pContext): 
    hangar(hangar), pContext(pContext) {
    setState(STARTUP);
}
  
void HangarTask::tick(){

    hangar->sync();

    if (pContext->getSystemState() == ALARM && state != ALARM) {
        setState(ALARM);
    }

    switch (state){   
        
        case STARTUP: {
            if (this->checkAndSetJustEntered()) {
                Logger.log("Dentro STARTUP");
                hangar->init();
            }
            setState(IDLE);
            break;
        }

        case IDLE: {
            if(this->checkAndSetJustEntered()) {
                Logger.log("Dentro IDLE");
            }

            if (pContext->isTakeOffRequest()) {
                Logger.log("Richiesta di TAKE OFF");
                if (pContext->isDroneInside()) {
                    setState(TAKE_OFF);
                } else {
                    Logger.log("ERRORE: Drone gia' fuori!");
                }
                pContext->setTakeOffRequest(false);
            } else if (pContext->isLandingRequest()) {
                Logger.log("CMD: M2 Received");
                if (!pContext->isDroneInside() && pContext->isDronePresent()) {
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
                hangar->startOpeningDoor();
                if (!pContext->isDoorMoving()) {

                }
            }

            if (pContext->getDroneDistance() > D1) {
                if (this->elapsedTimeInState() > T1) {
                    Logger.log("Manovra completata: Drone Out");
                    hangar->startClosingDoor();
                    pContext->setDroneInside(false);
                    setState(IDLE);
                }
            } else {
                this->stateTimestamp = millis();
            }
            break;
        }

        case LANDING: {
            if (this->checkAndSetJustEntered()) {
                hangar->startOpeningDoor();
                if (!pContext->isDoorMoving()) {

                }
            }

            if (pContext->getDroneDistance() < D2) {
                if (this->elapsedTimeInState() > T2) {
                    Logger.log("Manovra completata: Drone Inside");
                    hangar->startClosingDoor();
                    pContext->setDroneInside(true);
                    setState(IDLE);
                } 
            } else {
                this->stateTimestamp = millis();
            }
            break;
        }

        case ALARM: {
            if (this->checkAndSetJustEntered()) {
                Logger.log("!!! ALARM TRIGGERED !!!");
                hangar->activateAlarm(); 
            }

            if (hangar->checkResetButton()) {
                Logger.log("Alarm Reset Manuale");
                hangar->deactivateAlarm();
                pContext->setSystemState(NORMAL);
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