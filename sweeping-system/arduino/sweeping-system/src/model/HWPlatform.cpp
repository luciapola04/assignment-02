#include "HWPlatform.h"
#include <Arduino.h>
#include "devices/ButtonImpl.h"
#include "devices/LcdImpl.h"
#include "devices/Led.h"
#include "devices/ServoMotorImpl.h"
#include "devices/Pir.h"
#include "devices/SonarImpl.h"
#include "devices/TempSensorLM35.h" 
#include "config.h"

void wakeUp() {}

HWPlatform::HWPlatform(){
  resetButton = new ButtonImpl(RESET_PIN);
  lcd         = new LcdImpl(0x27, 16, 2); 
  L1          = new Led(L1_PIN);
  L2          = new Led(L2_PIN);
  L3          = new Led(L3_PIN);
  servoMotor  = new ServoMotorImpl(MOTOR_PIN);
  pir         = new Pir(PIR_PIN);
  sonar       = new SonarImpl(TRIG_PIN, ECHO_PIN);
  tempSensor  = new TempSensorLM35(TEMP_PIN); 
}

void HWPlatform::init(){
}

Button* HWPlatform::getButton(){
  return this->resetButton;
}

Lcd* HWPlatform::getLcd() {
  return this->lcd;
}

Led*  HWPlatform::getL1(){
  return this->L1;
}

Led*  HWPlatform::getL2(){
  return this->L2;
}

Led*  HWPlatform::getL3(){
  return this->L3;
}

ServoMotor* HWPlatform::getMotor(){
  return this->servoMotor;
}

Sonar* HWPlatform::getSonar(){
  return this->sonar;
}

TempSensor* HWPlatform::getTempSensor(){
  return this->tempSensor;
}

Pir* HWPlatform::getPir(){
  return this->pir;
}

/*
void HWPlatform::test(){
  bool btPressed = pButton->isPressed();
  pLed->switchOn();
  pMotor->on();
  pMotor->setPosition(90);
  Logger.log("Button: " + String(btPressed ? "pressed" : " not pressed"));
  delay(1000);
  pMotor->setPosition(0);
  delay(1000);
  pMotor->off();
  pLed->switchOff();
}
*/