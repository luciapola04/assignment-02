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
#include "Context.h"

#define MIN_DIST_DRONE 0.10
#define MAX_DIST_DRONE 1.00

DroneHangar::DroneHangar(HWPlatform* pHW, Context* pContext): 
    pHW(pHW), pContext(pContext) {
}

void DroneHangar::init(){
  lastDistance = MAX_DIST_DRONE;
  lastTemperature = 20; // some initial value: to be fixed
  this->reset();
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
    pContext->setDoorMoving(true);
  }
}

void DroneHangar::stopOpeningDoor(){
  pHW->getMotor()->off();
  pContext->setDoorMoving(false);
}

void DroneHangar::startClosingDoor(){
  pHW->getMotor()->on();
  pHW->getMotor()->setPosition(90);
  pContext->setDoorMoving(true);
}

void DroneHangar::stopClosingDoor(){
  pHW->getMotor()->off();
  pContext->setDoorMoving(false);
}


bool DroneHangar::isAvailable(){
  return isAvail;
}

void DroneHangar::activateAlarm(){
  pHW->getL3()->switchOn(); // Accende LED Rosso
  pHW->getL1()->switchOff(); // Spegne LED Verde (opzionale)
        
  // Chiude la porta se aperta (Logica di sicurezza immediata)
  pHW->getMotor()->on();
  pHW->getMotor()->setPosition(90); // Chiude
  pContext->setDoorMoving(false); //stop lamopeggi
}

void DroneHangar::deactivateAlarm() {
  pHW->getL3()->switchOff();
  pHW->getL1()->switchOn();
  pHW->getMotor()->off();
}

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
