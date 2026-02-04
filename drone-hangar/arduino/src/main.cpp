#include <Arduino.h>
#include "HWPlatform.h"
#include "Context.h"
#include "tasks/HangarTask.h"

HWPlatform* hw;
Context* context;
HangarTask* hangarTask;

void setup() {
  Serial.begin(115200);
  
  // Inizializza l'hardware e il contesto
  hw = new HWPlatform();
  hw->init();
  context = new Context();

  // Inizializza il task dell'hangar con un periodo di 100ms
  // Questa funzione 'init' viene dalla classe base Task
  hangarTask = new HangarTask(hw, context);
  hangarTask->init(100); 
}

void loop() {
  // Lo scheduler controlla se sono passati 100ms
  // La funzione updateAndCheckTime è definita in Task.h
  if (hangarTask->updateAndCheckTime(10)) { 
    hangarTask->tick(); 
  }
  
  delay(10); // Piccolo delay per stabilità dello scheduler
}