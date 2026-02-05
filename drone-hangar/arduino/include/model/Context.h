#include "HWPlatform.h"
#include "UserPanel.h"
#ifndef __CONTEXT__
#define __CONTEXT__

enum HangarState {IDLE, TAKE_OFF, LANDING, ALARM_STATE};
enum AlarmState { AT_NORMAL, CHECKING_PRE_ALARM, PRE_ALARM, CHECKING_ALARM, ALARM };

class Context {
    public:
        Context(HWPlatform* pHW);
        void init();

        void setAlarmState(AlarmState s);
        AlarmState getAlarmState();

        
    
        float getCurrentTemp();

        //sonar
        float getDroneDistance();
        void setDroneDistance(float d);

        //pir se il drone fuori
        void setDronePresent(bool present);
        bool isDronePresent();

        //se il drone è dentro/fuori
        bool isDroneInside();
        void setDroneInside(bool inside);


        void setDoorOpen(bool open);
        bool isDoorOpen();

        bool isInPreAlarm();
        void setPreAlarm(bool preAlarm);
        bool isInAlarm();
        void setAlarm(bool alarm);

        bool isTakeOffRequest();
        void setTakeOffRequest(bool req);
        bool isLandingRequest();
        void setLandingRequest(bool req);

        bool isInLanding();
        bool isInTakeOff();
        void setLanding(bool landing);
        void setTakeOff(bool takeoff);

        float getTemperature();

    private:
        AlarmState alarmState;
        HWPlatform *pHW;

        bool dronePresent; 

        bool droneInside;  
        
        bool reqTakeOff;   
        bool reqLanding;  

        bool inPreAlarm;
        bool inAlarm;

        float currentTemp;
        float currentDistance;

        bool doorOpen;

        bool landing;
        bool takeOff;
    };
    
    #endif