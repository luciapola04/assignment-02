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
                pContext->setPir(P_ON);
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
                pContext->setDoorCommand(CMD_OPEN);
                pContext->setSonar(S_ON);
                pContext->setPir(P_OFF);
            }

            if (pContext->getDoorCommand() == CMD_OPEN) {
                if (pContext->isDoorOpen()) {
                    if (pContext->getDroneDistance() > D1) {
                        if (elapsedTimeInState() > (T_DOOR_MOVE + T1)) {
                            Logger.log("Drone Uscito. Chiudo...");
                            pContext->setDoorCommand(CMD_CLOSE);
                            pContext->setDroneInside(false);
                        }
                    }
                }
            } else if (pContext->getDoorCommand() == CMD_CLOSE) {
                if (pContext->isDoorClose()) {
                    Logger.log("Chiusura completata");
                    pContext->setSonar(S_OFF);
                    setState(IDLE);
                }
            }
            break;
        }

        case LANDING: {
            if (this->checkAndSetJustEntered()) {
                Logger.log("Dentro LANDING");
                pContext->setSonar(S_ON);
                pContext->setDoorCommand(CMD_OPEN);
            }

            if (pContext->getDoorCommand() == CMD_OPEN) {
                if (pContext->isDoorOpen()) {
                    if (pContext->getDroneDistance() < D2) {
                        if (elapsedTimeInState() > (T_DOOR_MOVE + T2)) {
                            Logger.log("Drone Entrato. Chiudo...");
                            pContext->setDoorCommand(CMD_CLOSE);
                            pContext->setDroneInside(true);
                        }
                    }
                }
            } else if (pContext->getDoorCommand() == CMD_CLOSE) {
                if (pContext->isDoorClose()) {
                    Logger.log("Chiusura completata");
                    pContext->setSonar(S_OFF);
                    pContext->setPir(P_OFF);
                    setState(IDLE);
                }
            }
            break;
        }

        case ALARM_STATE: {
            if (this->checkAndSetJustEntered()) {
                Logger.log("ALLARME!!!");
                pContext->activateAlarm();
                pContext->setDoorCommand(CMD_CLOSE);
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