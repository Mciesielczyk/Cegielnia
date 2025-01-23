#include "truck.h"
#include <iostream>
#include <unistd.h>

Truck::Truck(int id, ConveyorBelt& conveyor) : id(id), conveyor(conveyor) {}

void Truck::run() {
    while (!conveyor.is_done()) {
        std::cout << "Ciężarówka " << id << " załadowana cegłami." << std::endl;
        conveyor.load_truck(id);  // Ładowanie cegieł na ciężarówkę
        sleep(2);  // Czas transportu
    }
}
