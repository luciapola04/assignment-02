#include "DroneHangar.h"
#include <Arduino.h>
#include "devices/ButtonImpl.h"
#include "kernel/MsgService.h"
#include <avr/sleep.h>
#include "config.h"
#include "devices/Led.h"
#include "devices/LightSensorImpl.h"
#include "devices/TempSensorLM35.h"
#include "devices/ServoMotorImpl.h"
#include "kernel/Logger.h"

#define MIN_DIST_DRONE 0.10
#define MAX_DIST_DRONE 1.00

DroneHangar::DroneHangar(HWPlatform* pHW):pHW(pHW){
}

void DroneHangar::init(){
  lastDistance = MAX_DIST_DRONE;
  lastTemperature = 20; // some initial value: to be fixed
  this->reset();
  isAvail = true;
  preAlarmState = false;
  alarmState = false;
  pHW->getMotor()->on();
  pHW->getL1()->switchOn();
  pHW->getL3()->switchOff();
  pHW->getMotor()->off();
}
  
void DroneHangar::sync(){
  float dist = lastDistance; 
  currentTemp = lastTemperature;
  
  if (sensorsCanBeUsed()){
    currentTemp = pHW->getTempSensor()->getTemperature();
    dist = pHW->getSonar()->getDistance();
    if (dist == NO_OBJ_DETECTED){
      dist = MAX_DIST_DRONE;
    }
    lastDistance = dist; 
    lastTemperature = currentTemp;
  }
  

  static int localCounter = 0;    
  localCounter++;
  if (localCounter % 25 == 0){
    Logger.log("[WC] Temp: " + String(currentTemp).substring(0,5) + " Dist: " + String(dist));
  }
}

bool DroneHangar::sensorsCanBeUsed(){
  return !pHW->getMotor()->isOn();
}

void DroneHangar::startOpeningDoor(){
  if (isAvail){
    pHW->getMotor()->on();
    pHW->getMotor()->setPosition(180);
  }
}

void DroneHangar::stopOpeningDoor(){
  pHW->getMotor()->off();
}

void DroneHangar::startClosingDoor(){
  pHW->getMotor()->on();
  pHW->getMotor()->setPosition(90);
}

void DroneHangar::stopClosingDoor(){
  pHW->getMotor()->off();
}


bool DroneHangar::isAvailable(){
  return isAvail;
}

bool DroneHangar::isInMaintenance(){
  return !isAvail;
}

void DroneHangar::setPreAlarm(bool active){
    preAlarmState = active;
    // In Pre-Alarm non cambiamo luci o porte, inibiamo solo i voli futuri
    // Questa variabile verrà letta dall'HangarTask
}

void DroneHangar::setAlarm(bool active){
    alarmState = active;
    if(active){
        pHW->getL3()->switchOn(); // Accende LED Rosso
        pHW->getL1()->switchOff(); // Spegne LED Verde (opzionale)
        
        // Chiude la porta se aperta (Logica di sicurezza immediata)
        pHW->getMotor()->on();
        pHW->getMotor()->setPosition(90); // Chiude
        // Nota: Idealmente dovremmo aspettare che chiuda, ma in emergenza diamo il comando.
    } else {
        pHW->getL3()->switchOff();
        pHW->getL1()->switchOn();
        pHW->getMotor()->off();
    }
}

bool DroneHangar::isPreAlarm() { return preAlarmState; }
bool DroneHangar::isAlarm() { return alarmState; }

bool DroneHangar::checkResetButton(){
    return pHW->getButton()->isPressed();
}


void DroneHangar::reset(){
  isAvail = true;
  pHW->getL1()->switchOn();
  pHW->getL3()->switchOff();
  pHW->getMotor()->off();
}

float DroneHangar::getCurrentTemperature(){
  return this->currentTemp;
}
