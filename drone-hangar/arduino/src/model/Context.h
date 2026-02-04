#include "HWPlatform.h"
#ifndef __CONTEXT__
#define __CONTEXT__

enum SystemState { NORMAL, PRE_ALARM, ALARM };

class Context {
    public:
        Context(HWPlatform* pHW);
        void reset();

        void sync();
        
        void setSystemState(SystemState s);
        SystemState getSystemState();
    
        float getCurrentTemp();
        float getDroneDistance();
        bool isDronePresent();
        bool isDroneInside();
        void setDroneInside(bool inside);
        bool isDoorOpen();

        bool isInPreAlarm();
        void setPreAlarm(bool preAlarm);
        bool isInAlarm();
        void setAlarm(bool alarm);

        bool isTakeOffRequest();
        void setTakeOffRequest(bool req);
        bool isLandingRequest();
<<<<<<< Updated upstream
        bool checkResetButtonAndReset();
=======
        void setLandingRequest(bool req);
        bool checkResetButton();
>>>>>>> Stashed changes
        float getTemperature();

        void openDoor();
        void closeDoor();

        void activateAlarm();
        void deactivateAlarm();


    
    private:
        bool started;
        bool stopped;
        SystemState systemState;
        HWPlatform* pHW;
        float currentTemp;
        float droneDistance;
        
        bool dronePresent; 
        bool droneInside;  
        
        bool doorOpen;   

        bool reqTakeOff;   
        bool reqLanding;  

        bool buttonReset; 

        bool inPreAlarm;
        bool inAlarm;

        float currentTemp;
        float currentDistance;
    };
    
    #endif