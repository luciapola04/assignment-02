#ifndef __SERIALTASK__
#define __SERIALTASK__

#include "kernel/Task.h"
#include "Context.h"

class SerialMonitorTask : public Task
{

public:
    SerialMonitorTask(Context* pContext);
    void init(int period);
    void tick();

private:
    Context* pContext;
    void sendSystemState();
};

#endif