#include "SerialMonitorTask.h"
#include "kernel/MsgService.h"
#include "kernel/Logger.h"
#include <Arduino.h>

SerialMonitorTask::SerialMonitorTask(Context* pContext): pContext(pContext){
}

void SerialMonitorTask::init(int period){
  Task::init(period);
  MsgService.init();
}

void SerialMonitorTask::tick(){
  
  if (MsgService.isMsgAvailable()) {
    Msg* msg = MsgService.receiveMsg();    
    if (msg != NULL){
      String content = msg->getContent();
      content.trim();

      Logger.log("Received cmd: " + content);

      if (content == "takeoff-req"){
        pContext->setTakeOffRequest(true);
      } 
      else if (content == "landing-req"){
        pContext->setLandingRequest(true);
      }
      
      delete msg; 
    }
  }
  //sendSystemState();
}

void SerialMonitorTask::sendSystemState(){
  
  String droneStateStr = "UNKNOWN";
  String hangarStateStr = "NORMAL";

  if (pContext->isInAlarm()) {
    hangarStateStr = "ALARM";
  } else if (pContext->isInPreAlarm()) {
    hangarStateStr = "PRE-ALARM";
  }

  // --- Determina lo stato del Drone (Logica di visualizzazione) ---
  // Priorità: Allarme -> Movimento Porta -> Posizione
  if (pContext->isInAlarm()) {
    droneStateStr = "HALTED"; 
  } 
  else if (pContext->isDoorOpen()) {
    // Se la porta si muove e il drone era dentro, sta decollando
    // Se la porta si muove e il drone era fuori, sta atterrando
    if (pContext->isDroneInside()) {
        droneStateStr = "TAKE OFF";
    } else {
        droneStateStr = "LANDING";
    }
  } 
  else if (pContext->isDroneInside()) {
    droneStateStr = "DRONE INSIDE";
  } 
  else {
    droneStateStr = "DRONE OUT";
  }

  // --- Recupera la distanza ---
  float distance = pContext->getDroneDistance();

  // --- Costruisci il messaggio Protocollo ---
  // Formato: dh:st:StatoDrone:StatoHangar:Distanza
  String msg = "dh:st:" + droneStateStr + ":" + hangarStateStr + ":" + String(distance);
  
  MsgService.sendMsg(msg);
}