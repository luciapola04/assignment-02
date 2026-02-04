#include "Context.h"
#include <kernel/Logger.h>

Context::Context(HWPlatform* pHW){
    this->pHW=pHW;
    this->reset();
}

void Context::reset(){
    this->started = false;
    this->stopped = true; 
    this->systemState = NORMAL;
    this->currentTemp = 0.0;
    this->droneDistance = 0.0;
    this->dronePresent = false;
    this->droneInside = true;
    this->doorOpen = false;
    this->inPreAlarm = false;

    this->reqTakeOff = false;
    this->reqLanding = false;
    this->buttonReset = false;

    this -> currentDistance = 0;
    this -> currentTemp = 15;

    pHW->getL1()->switchOn();
    pHW->getL3()->switchOff();
    pHW->getMotor()->off();
}

void Context::sync(){
  
  this->currentTemp = pHW->getTempSensor()->getTemperature();
  this->currentDistance = pHW->getSonar()->getDistance();
  this->buttonReset = pHW->getButton()->isPressed();
  if (currentDistance == NO_OBJ_DETECTED){
    currentDistance = 1000; //da cambiare distanza quando non rileva nulla
  }

  static int localCounter = 0;    
  localCounter++;
  if (localCounter % 25 == 0){
    Logger.log("[WC] Temp: " + String(currentTemp).substring(0,5) + " Dist: " + String(currentDistance));
  }
  
}

void Context::openDoor(){
    pHW->getMotor()->on();
    pHW->getMotor()->setPosition(180);
    this->doorOpen = true;
    pHW->getMotor()->off();
}

void Context::closeDoor(){
    pHW->getMotor()->on();
    pHW->getMotor()->setPosition(90);
    this->doorOpen = false;
    pHW->getMotor()->off();
}

void Context::activateAlarm(){
  pHW->getL3()->switchOn();
  pHW->getL1()->switchOff();

  if(this->doorOpen){
    this->closeDoor();
  }
}

void Context::deactivateAlarm() {
  pHW->getL3()->switchOff();
  pHW->getL1()->switchOn();
  pHW->getMotor()->off();
}

bool Context::checkResetButtonAndReset(){
    if(buttonReset) buttonReset=false; return true;
    return buttonReset;
}


void Context::setSystemState(SystemState s) { this->systemState = s; }
SystemState Context::getSystemState() { return this->systemState; }

float Context::getCurrentTemp() { return this->currentTemp; }
float Context::getDroneDistance() { return this->droneDistance; }
bool Context::isDronePresent() { return this->dronePresent; }
bool Context::isDroneInside() { return this->droneInside; }
void Context::setDroneInside(bool inside) { this->droneInside = inside; }
bool Context::isDoorOpen() { return this->doorOpen; }

bool Context::isInPreAlarm() { return this->inPreAlarm; }
void Context::setPreAlarm(bool preAlarm){ this->inPreAlarm = preAlarm;}
bool Context::isInAlarm() {return this->inAlarm;}
void Context::setAlarm(bool alarm){ this->inAlarm = alarm;}

bool Context::isTakeOffRequest() { return this->reqTakeOff; }
void Context::setTakeOffRequest(bool req) { this->reqTakeOff = req; }
bool Context::isLandingRequest() { return this->reqLanding; }
void Context::setLandingRequest(bool req) { this->reqLanding = req; }
float Context::getTemperature() { return this->currentTemp;}


