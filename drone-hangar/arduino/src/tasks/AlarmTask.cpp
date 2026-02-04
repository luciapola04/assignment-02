#include "AlarmTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

#define TEMP1 20.0
#define TEMP2 40.0 
#define TIME_T3 3000
#define TIME_T4 2000

AlarmTask::AlarmTask(DroneHangar* droneHangar): 
    droneHangar(droneHangar){
    setState(NORMAL);
}
  
void AlarmTask::tick(){
    
    droneHangar->sync();
    float currentTemp = droneHangar->getCurrentTemperature();

    switch (state){    
        
    case NORMAL: {
        if (checkAndSetJustEntered()){
            Logger.log(F("[ALARM] System Normal"));
            droneHangar->setPreAlarm(false);
            droneHangar->setAlarm(false);
        }
        
        // Se supera Temp1, inizia a contare il tempo T3
        if (currentTemp >= TEMP1){
            setState(CHECKING_PRE_ALARM);
        }
        break;
    }

    case CHECKING_PRE_ALARM: {
        
        if (currentTemp < TEMP1){
            setState(NORMAL);
        } 
        else if (elapsedTimeInState() > TIME_T3){
            setState(PRE_ALARM);
        }
        break;       
    }

    case PRE_ALARM: {
        if (checkAndSetJustEntered()){
            Logger.log(F("[ALARM] Pre-Alarm State"));
            droneHangar->setPreAlarm(true);
        }
        if (currentTemp < TEMP1){
            setState(NORMAL);
        }
        else if (currentTemp >= TEMP2){
            setState(CHECKING_ALARM);
        }
        break;
    }

    case CHECKING_ALARM: {
        if (currentTemp < TEMP2){
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
            droneHangar->setAlarm(true);
            // Inviare messaggio via DRU (da gestire nel CommunicatorTask leggendo lo stato)
        }

        //solo con tasto RESET.
        if (droneHangar->checkResetButton()){
            Logger.log(F("[ALARM] Reset pressed."));
            setState(NORMAL);
        }
        break;
    }
    
    }
}

void AlarmTask::setState(State s){
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
