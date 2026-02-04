#include "Context.h"

Context::Context(){
    this->reset();
}

void Context::reset(){
    this->started = false;
    this->stopped = true; 
    this->systemState = NORMAL;
    this->currentTemp = 0.0;
    this->droneDistance = 0.0;
    this->dronePresent = false;
}

bool Context::isStarted() { return this->started; }
bool Context::isStopped() { return this->stopped; }

void Context::setStarted(){
    this->started = true;
    this->stopped = false;
}

void Context::setStopped(){
    this->stopped = true;
    this->started = false;
}

void Context::setSystemState(SystemState s) { this->systemState = s; }
SystemState Context::getSystemState() { return this->systemState; }

void Context::setCurrentTemp(float t) { this->currentTemp = t; }
float Context::getCurrentTemp() { return this->currentTemp; }

void Context::setDroneDistance(float d) { this->droneDistance = d; }
float Context::getDroneDistance() { return this->droneDistance; }

void Context::setDronePresent(bool present) { this->dronePresent = present; }
bool Context::isDronePresent() { return this->dronePresent; }

void Context::setDoor(bool state) { this->dronePresent = state; }
bool Context::isDoorOpen() { return this->dronePresent; }

void Context::setAlarm(bool state) { this->dronePresent = state; }
bool Context::isInPreAlarm() { return this->dronePresent; }