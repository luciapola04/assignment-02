#include "tasks/AlarmTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"
#include "model/Context.h"

#define TEMP1 55.0
#define TEMP2 60.0 
#define TIME_T3 3000
#define TIME_T4 3000

AlarmTask::AlarmTask(HWPlatform* pHw, Context* pContext, UserPanel* pUserPanel): 
    pContext(pContext), pHw(pHw), pUserPanel(pUserPanel)  {
    setState(AT_NORMAL);
}
  
void AlarmTask::tick(){
    
    this->currentTemp = pHw->getTempSensor()->getTemperature();
    static int localCounter = 0;    
    localCounter++;
    if (localCounter % 25 == 0){
        Logger.log("[WC] Temp: " + String(currentTemp).substring(0,5));
    }

    switch (state){    
    case AT_NORMAL: {
        if (checkAndSetJustEntered()){
            Logger.log(F("[ALARM] System Normal"));
            pContext->setPreAlarm(false);
            pContext->setAlarm(false);
        }
        
        // Se supera Temp1, inizia a contare il tempo T3
        if (this->currentTemp >= TEMP1){
            setState(CHECKING_PRE_ALARM);
        }
        break;
    }

    case CHECKING_PRE_ALARM: {
        
        if (this->currentTemp < TEMP1){
            setState(AT_NORMAL);
        } 
        else if (elapsedTimeInState() > TIME_T3){
            setState(PRE_ALARM);
        }
        break;       
    }

    case PRE_ALARM: {
        if (checkAndSetJustEntered()){
            Logger.log(F("[ALARM] Pre-Alarm State"));
            pContext->setPreAlarm(true);
        }
        if (this->currentTemp < TEMP1){
            setState(AT_NORMAL);
        }
        else if (this->currentTemp >= TEMP2){
            setState(CHECKING_ALARM);
        }
        break;
    }

    case CHECKING_ALARM: {
        if (this->currentTemp < TEMP2){
            setState(PRE_ALARM);
        }
        else if (elapsedTimeInState() > TIME_T4){
            setState(ALARM);
        }
        break;
    }

    case ALARM: {
        if (checkAndSetJustEntered()){
            Logger.log(F("[ALARM] CRITICAL ALARM!"));
            pContext->setAlarm(true);
            pUserPanel->displayAlarm();
            pHw->getL3()->switchOn();
            pHw->getL1()->switchOff();
            // Inviare messaggio via DRU (da gestire nel CommunicatorTask leggendo lo stato)
        }

        //solo con tasto RESET.
        if (pHw->getButton()->isPressed()){
            Logger.log(F("[ALARM] Reset pressed."));
            pContext->setAlarm(false);
            pHw->getL3()->switchOff();
            pHw->getL1()->switchOn();
            pHw->getMotor()->off(); 
            setState(AT_NORMAL);
        }
        break;
    }
    
    }
}

void AlarmTask::setState(AlarmState s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long AlarmTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool AlarmTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}
