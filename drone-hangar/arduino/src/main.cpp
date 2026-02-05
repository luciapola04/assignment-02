#include <Arduino.h>
#include "HWPlatform.h"
#include "Context.h"
#include "tasks/HangarTask.h"
#include "tasks/SerialMonitorTask.h"
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

  context = new Context(hw);

  Task* serialTask = new SerialMonitorTask(context);
  serialTask->init(150);

  sched.addTask(serialTask);
}

void loop() {
  sched.schedule();
}