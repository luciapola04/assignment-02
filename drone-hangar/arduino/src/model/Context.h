#ifndef __CONTEXT__
#define __CONTEXT__

enum SystemState { NORMAL, PRE_ALARM, ALARM };

class Context {
public:
    Context();
    void setStarted();
    void setStopped();
    bool isStarted();  
    bool isStopped();
    void setSystemState(SystemState s);
    SystemState getSystemState();
    void setDroneDistance(float d);
    float getDroneDistance();
    void setDroneInside(bool inside);
    bool isDroneInside();
    void setDronePresent(bool present);
    bool isDronePresent();
    void setCurrentTemp(float t);
    
    void setAlarm(bool state);
    bool isInPreAlarm();
    void setDoor(bool state);
    bool isDoorOpen();
    float getCurrentTemp();
    void reset();

private:
    bool started; 
    bool stopped;
    SystemState systemState;
    float droneDistance;
    bool droneInside;
    bool dronePresent;
    float currentTemp; 
    bool doorOpen;
    bool alarm;
};

#endif