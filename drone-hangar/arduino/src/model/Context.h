#ifndef __CONTEXT__
#define __CONTEXT__

enum SystemState { NORMAL, PRE_ALARM, ALARM };

class Context {
    public:
        Context();
        void reset();
    
        bool isStarted();
        bool isStopped();
        void setStarted();
        void setStopped();
        
        void setSystemState(SystemState s);
        SystemState getSystemState();
    
        void setCurrentTemp(float t);
        float getCurrentTemp();
    
        void setDroneDistance(float d);
        float getDroneDistance();
    
        void setDronePresent(bool present); 
        bool isDronePresent();
    
        bool isDroneInside();
        void setDroneInside(bool inside);
    
        void setDoorMoving(bool moving);
        bool isDoorMoving();
    
        void setPreAlarm(bool active);
        bool isInPreAlarm();
    
        void setTakeOffRequest(bool req);
        bool isTakeOffRequest();
        
        void setLandingRequest(bool req);
        bool isLandingRequest();
    
    private:
        bool started;
        bool stopped;
        SystemState systemState;
        float currentTemp;
        float droneDistance;
        
        bool dronePresent; 
        bool droneInside;  
        
        bool doorMoving;   
        bool inPreAlarm;   
        bool reqTakeOff;   
        bool reqLanding;   
    };
    
    #endif