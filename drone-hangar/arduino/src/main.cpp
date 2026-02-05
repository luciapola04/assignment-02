#include <Arduino.h>
#include "HWPlatform.h"
#include "Context.h"
#include "tasks/HangarTask.h"
#include "tasks/SerialMonitorTask.h"
#include "tasks/AlarmTask.h"
#include "tasks/BlinkingTask.h"
#include "tasks/DoorTask.h"
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

  Task* hangarTask = new HangarTask(hw,context);
  hangarTask->init(150);

  Task* blinkingTask = new BlinkingTask(hw->getL2(),context);
  blinkingTask->init(150);

  Task* alarmTask = new AlarmTask(hw,context);
  alarmTask->init(150);

  Task* doorTask = new DoorTask(hw->getMotor(),context);
  doorTask->init(150);

  

  sched.addTask(serialTask);
  sched.addTask(hangarTask);
  sched.addTask(blinkingTask);
  sched.addTask(alarmTask);
  sched.addTask(doorTask);
}

void loop() {
  sched.schedule();
}