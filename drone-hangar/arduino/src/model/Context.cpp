#include "Context.h"
#include <kernel/Logger.h>

Context::Context(HWPlatform* pHW){
    this->pHW=pHW;
    this->reset();
}

void Context::reset(){
    this->started = false;
    this->stopped = true; 
    this->systemState = AT_NORMAL;
    this->currentTemp = 0.0;
    this->currentDistance = 0.0;
    this->dronePresent = false;
    this->droneInside = true;
    this->doorCommand = CMD_CLOSE;
    this->doorStatus = D_CLOSE;
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
  this->buttonReset = pHW->getButton()->isPressed();
  static int localCounter = 0;    
  localCounter++;
  if (localCounter % 25 == 0){
    Logger.log("[WC] Temp: " + String(currentTemp).substring(0,5) + " Dist: " + String(currentDistance));
  }

}

/*

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

  if(this->isDoorOpen()){
    this->closeDoor();
  }
}

void Context::deactivateAlarm() {
  pHW->getL3()->switchOff();
  pHW->getL1()->switchOn();
  pHW->getMotor()->off();
}

*/

bool Context::checkResetButtonAndReset(){
    if(buttonReset) {
      buttonReset=false; 
      return true;
    }
    return buttonReset;
}

void Context::setDoorCommand(DoorCommand doorCommand) { this->doorCommand = doorCommand; }
DoorCommand Context::getDoorCommand() { return this->doorCommand; }

void Context::setDoorStatus(DoorStatus doorStatus) { this->doorStatus = doorStatus; }
DoorStatus Context::getDoorStatus() { return this->doorStatus; }

bool Context::isDoorOpen() { return this->doorStatus == D_OPEN; }
bool Context::isDoorClose() { return this->doorStatus == D_CLOSE; }


void Context::setSystemState(SystemState s) { this->systemState = s; }
SystemState Context::getSystemState() { return this->systemState; }

float Context::getCurrentTemp() { return this->currentTemp; }
float Context::getDroneDistance() { return this->currentDistance; }
void Context::setDroneDistance(float d) { this->currentDistance = d; }
void Context::setDronePresent(bool present) { this->dronePresent = present; }
bool Context::isDronePresent() { return this->dronePresent; }
bool Context::isDroneInside() { return this->droneInside; }
void Context::setDroneInside(bool inside) { this->droneInside = inside; }

void Context::setSonar(SonarState s) { this->sonarState = s; }
bool Context::isSonarActive() { this->sonarState == S_ON ? true : false; }

void Context::setPir(PirState s) { this->pirState = s; }
bool Context::isPirActive() { this->pirState == S_ON ? true : false; }

bool Context::isInPreAlarm() { return this->inPreAlarm; }
void Context::setPreAlarm(bool preAlarm){ this->inPreAlarm = preAlarm;}
bool Context::isInAlarm() {return this->inAlarm;}
void Context::setAlarm(bool alarm){ this->inAlarm = alarm;}

bool Context::isTakeOffRequest() { return this->reqTakeOff; }
void Context::setTakeOffRequest(bool req) { this->reqTakeOff = req; }
bool Context::isLandingRequest() { return this->reqLanding; }
void Context::setLandingRequest(bool req) { this->reqLanding = req; }
float Context::getTemperature() { return this->currentTemp;}
