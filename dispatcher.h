#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "conveyor_belt.h"

class Dispatcher {
public:
    Dispatcher(ConveyorBelt& conveyor);
    void run();

private:
    ConveyorBelt& conveyor;
};

#endif
