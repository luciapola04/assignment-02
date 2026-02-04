#include "tasks/HangarTask.h"
#include "Arduino.h"
#include "kernel/Logger.h"
#include "config.h"

HangarTask::HangarTask(Context* pContext):pContext(pContext) {
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
                pContext->openDoor();
                isClosing = false;
                doorTimer = millis();
            }

            if (!isClosing) {
                if (millis() - doorTimer > T_DOOR_MOVE) {
                    if (pContext->getDroneDistance() > D1) {
                        if (elapsedTimeInState() > (T_DOOR_MOVE + T1)) {
                            Logger.log("Drone Uscito. Chiudo...");
                            pContext->closeDoor();
                            isClosing = true;
                            doorTimer = millis();
                        }
                    }
                }
            } else {
                if (millis() - doorTimer > T_DOOR_MOVE) {
                    Logger.log("Chiusura completata");
                    pContext->setDroneInside(false); 
                    setState(IDLE);
                }
            }
            break;
        }

        case LANDING: {
            if (this->checkAndSetJustEntered()) {
                pContext->openDoor();
                isClosing = false;
                doorTimer = millis();
            }

            if (!isClosing) {
                if (millis() - doorTimer > T_DOOR_MOVE) {
                    if (pContext->getDroneDistance() < D2) {
                        if (elapsedTimeInState() > (T_DOOR_MOVE + T2)) {
                            Logger.log("Drone Entrato. Chiudo...");
                            pContext->closeDoor();
                            isClosing = true;
                            doorTimer = millis();
                        }
                    }
                }
            } else {
                if (millis() - doorTimer > T_DOOR_MOVE) {
                    Logger.log("Chiusura completata");
                    pContext->setDroneInside(true); 
                    setState(IDLE);
                }
            }
            break;
        }

        case ALARM_STATE: {
            if (this->checkAndSetJustEntered()) {
                Logger.log("ALLARME!!!");
                pContext->activateAlarm(); 
            }

            if (pContext->checkResetButtonAndReset()) {
                Logger.log("RESETTING ALLARME");
                pContext->deactivateAlarm();
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