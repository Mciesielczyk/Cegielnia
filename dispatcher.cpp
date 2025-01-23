#include "dispatcher.h"
#include <iostream>
#include <unistd.h>

Dispatcher::Dispatcher(ConveyorBelt& conveyor) : conveyor(conveyor) {}

void Dispatcher::run() {
    std::cout << "Dyspozytor czeka na sygnał." << std::endl;
    sleep(5);  // Symulacja czekania na sygnał
    std::cout << "Dyspozytor zakończył pracę." << std::endl;
    conveyor.done = true;
}
