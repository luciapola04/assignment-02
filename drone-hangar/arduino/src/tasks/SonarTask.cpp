#include "SonarTask.h"
#include "kernel/Logger.h"
#include "config.h"

SonarTask::SonarTask(Sonar *pSonar, Context *pContext)
    : pSonar(pSonar), pContext(pContext)
{
    setState(S_OFF);

}

void SonarTask::tick() {
    switch (state)
    {
    case S_OFF:
        pContext->setDroneDistance(0.0);
        if(pContext->isSonarActive()) {
            setState(S_ON);
        }
        break;

    case S_ON:
        if(!pContext->isSonarActive()) {
            setState(S_OFF);
            return;
        }

        float currentDist = pSonar->getDistance();

        if (currentDist == NO_OBJ_DETECTED || currentDist < 0) {
            currentDist = MAX_D_DIST;
        }

        pContext->setDroneDistance(currentDist);
        break;
    }
}

void SonarTask::setState(SonarState s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long SonarTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool SonarTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}