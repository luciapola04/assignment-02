#include <Arduino.h>
#include "HWPlatform.h"
#include "Context.h"
#include "tasks/HangarTask.h"
#include "tasks/BlinkingTask.h"
#include "kernel/Scheduler.h"
#include "kernel/Logger.h"

#define BASE_PERIOD 50

HWPlatform* hw;
Context* context;

Scheduler sched;

void setup() {
  Serial.begin(115200);

  sched.init(BASE_PERIOD);
  Logger.log("Drone hangar");

  hw = new HWPlatform();
  hw->init();

  //utilizzato per variabili di controllo
  context = new Context();

  Task* hangarTask = new HangarTask(hw, context);
  hangarTask->init(100); 

  Task* blinkingTask = new BlinkingTask(hw->getL2(),context);
  blinkingTask->init(150);

  sched.addTask(hangarTask);
  sched.addTask(blinkingTask);
}

void loop() {
  sched.schedule();
}