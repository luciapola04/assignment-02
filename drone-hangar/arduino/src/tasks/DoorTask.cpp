#include "tasks/DoorTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"



DoorTask::DoorTask(ServoMotor* pMotor, Context* pContext): 
    pMotor(pMotor), pContext(pContext){
    setState(CLOSED);
}

void DoorTask::tick(){

    DoorCommand cmd = pContext->getDoorCommand();

    switch (state){
        case OPEN: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("DOOR OPEN"));
                pContext->setDoorStatus(D_OPEN);
            }

            if (cmd == CMD_CLOSE) {
                setState(CLOSING);
            }

            /*
            if (pContext->isInAlarm() ||
                (!pContext->isDroneInside() && pContext->isTakeOffRequest()) ||
                (pContext->isDroneInside() && pContext->isLandingRequest())) {
                setState(CLOSING);
            }
            */
        break;
    }
    
        case OPENING: {        
            if (this->checkAndSetJustEntered()){
                Logger.log(F("DOOR IS OPENING"));
                pContext->setDoorStatus(D_MOVING);
                pMotor->on();
                pMotor->setPosition(MOTOR_OPEN_POS);
            }
            
            if (this->elapsedTimeInState() > T_DOOR_MOVE) {
                pMotor->off();
                setState(OPEN);
            }
            break;
        }

        case CLOSED: {   
            if (this->checkAndSetJustEntered()){
                Logger.log(F("DOOR CLOSED"));
                pContext->setDoorStatus(D_CLOSE);
            }

            if (cmd == OPEN) {
                setState(OPENING);
            }

            /*
            if (!pContext->isInAlarm() &&
                (pContext->isDroneInside() && pContext->isTakeOffRequest()) ||
                (!pContext->isDroneInside() && pContext->isLandingRequest())) {
                setState(OPENING);
            }
            */
            break;
        }

        case CLOSING: {        
            if (this->checkAndSetJustEntered()){
                Logger.log(F("DOOR IS CLOSING"));
                pContext->setDoorStatus(D_MOVING);
                pMotor->on();
                pMotor->setPosition(MOTOR_CLOSE_POS);
            }
            
            if (this->elapsedTimeInState() > T_DOOR_MOVE) {
                pMotor->off();
                setState(CLOSED);
            }
            break;
        }
    }
}

void DoorTask::setState(MotorState s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long DoorTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool DoorTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}
