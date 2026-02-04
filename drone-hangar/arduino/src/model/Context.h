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
    bool isDroneInside();
    void setDroneInside(bool inside);
    float getDroneDistance();
    void setDronePresent(bool present);
    bool isDronePresent();
    void setCurrentTemp(float t);
    float getCurrentTemp();
    void reset();

    void setAlarm(bool state);
    void setDoor(bool state);
    bool isDoorOpen();
    bool isInPreAlarm();

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