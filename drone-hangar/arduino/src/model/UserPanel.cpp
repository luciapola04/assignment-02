#include "model/UserPanel.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"
#include "model/HWPlatform.h"

UserPanel::UserPanel(HWPlatform* pHW) {
    pLcd = pHW->getLcd();
}

void UserPanel::init(){
  pLcd->init();
  pLcd->backlight();
  pLcd->noDisplay();
  turnOnDisplay();
}

void UserPanel::turnOnDisplay(){
  pLcd->display();
  pLcd->clear();
}

void UserPanel::turnOffDisplay(){
  pLcd->noDisplay();
}

void UserPanel::displayDroneInside(){
  pLcd->clear();
  pLcd->setCursor(0, 0); 
  pLcd->print("DRONE INSIDE");
}

void UserPanel::displayTakeOff(){
  pLcd->clear();
  pLcd->setCursor(0, 0); 
  pLcd->print("TAKE OFF");
}

void UserPanel::displayDroneOut(){
  pLcd->clear();
  pLcd->setCursor(0, 0); 
  pLcd->print("DRONE OUT");
}

void UserPanel::displayLanding(){
  pLcd->clear();
  pLcd->setCursor(0, 0); 
  pLcd->print("LANDING");
}

void UserPanel::displayAlarm(){
  pLcd->clear();
  pLcd->setCursor(0, 0); 
  pLcd->print("ALARM");
}
