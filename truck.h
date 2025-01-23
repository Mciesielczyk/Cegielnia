#ifndef TRUCK_H
#define TRUCK_H

#include "conveyor_belt.h"

class Truck {
public:
    Truck(int id, ConveyorBelt& conveyor);
    void run();

private:
    int id;
    ConveyorBelt& conveyor;
};

#endif
