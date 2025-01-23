#ifndef WORKER_H
#define WORKER_H

#include "conveyor_belt.h"

class Worker {
public:
    Worker(ConveyorBelt& belt, int brickWeight);
    void start();

    void stop();  // Funkcja do zatrzymywania pracownika

private:
    ConveyorBelt& belt;
    int brickWeight;
    bool running;  // Zmienna odpowiedzialna za to, czy pracownik ma dodawać cegłę
};

#endif // WORKER_H
