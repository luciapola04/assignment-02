#include "PirTask.h"
#include "kernel/Logger.h"
#include "config.h"

PirTask::PirTask(Pir *pPir, Context *pContext)
    : pPir(pPir), pContext(pContext)
{
    setState(P_OFF);

}

void PirTask::tick() {
    switch (state)
    {
    case S_OFF:
        pContext->setDronePresent(false);
        if (pContext->isPirActive()) {
            setState(P_ON);
        }
        break;

    case S_ON:
        if(!pContext->isPirActive()) {
            setState(P_OFF);
            return;
        }

        pContext->setDronePresent(pPir->isDetected());
        break;
    }
}

void PirTask::setState(PirState s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long PirTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool PirTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}