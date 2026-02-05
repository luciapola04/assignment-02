#include "HWPlatform.h"
#ifndef __CONTEXT__
#define __CONTEXT__

enum HangarState {STARTUP, IDLE, TAKE_OFF, LANDING, ALARM_STATE};
enum SystemState { AT_NORMAL, CHECKING_PRE_ALARM, PRE_ALARM, CHECKING_ALARM, ALARM };
enum DoorCommand { CMD_OPEN, CMD_CLOSE };
enum DoorStatus { D_CLOSE, D_MOVING, D_OPEN };
enum SonarState { S_OFF, S_ON };
enum PirState { P_OFF, P_ON };

class Context {
    public:
        Context(HWPlatform* pHW);
        void reset();

        void sync();
        
        void setSystemState(SystemState s);
        SystemState getSystemState();

        
    
        float getCurrentTemp();
        float getDroneDistance();
        void setDroneDistance(float d);
        void setDronePresent(bool present);
        bool isDronePresent();
        bool isDroneInside();
        void setDroneInside(bool inside);

        DoorStatus getDoorStatus();
        void setDoorStatus(DoorStatus doorStatus);
        DoorCommand getDoorCommand();
        void setDoorCommand(DoorCommand doorcommand);
        bool isDoorOpen();
        bool isDoorClose();

        void setSonar(SonarState s);
        bool isSonarActive();

        void setPir(PirState s);
        bool isPirActive();

        bool isInPreAlarm();
        void setPreAlarm(bool preAlarm);
        bool isInAlarm();
        void setAlarm(bool alarm);

        bool isTakeOffRequest();
        void setTakeOffRequest(bool req);
        bool isLandingRequest();
        void setLandingRequest(bool req);
        bool checkResetButtonAndReset();
        float getTemperature();

        void openDoor();
        void closeDoor();

        void activateAlarm();
        void deactivateAlarm();


    
    private:
        bool started;
        bool stopped;
        SystemState systemState;
        DoorCommand doorCommand;
        DoorStatus doorStatus;
        HWPlatform *pHW;

        SonarState sonarState;

        PirState pirState;

        bool dronePresent; 
        bool droneInside;  
        
        bool reqTakeOff;   
        bool reqLanding;  

        bool buttonReset; 

        bool inPreAlarm;
        bool inAlarm;

        float currentTemp;
        float currentDistance;
    };
    
    #endif