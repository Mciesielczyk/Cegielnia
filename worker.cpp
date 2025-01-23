#include "worker.h"
#include <unistd.h>
#include <iostream>

// Konstruktor klasy Worker
Worker::Worker(ConveyorBelt& belt, int brickWeight)
    : belt(belt), brickWeight(brickWeight), running(true) {}

// Funkcja startowa, tworzy proces, który dodaje cegłę do taśmy
void Worker::start() {
    pid_t pid = fork();
    if (pid == 0) {  // Proces potomny
        while (running) {  // Dopóki running jest true, dodawaj cegłę
                addBrick(&belt, brickWeight);  // Dodaje cegłę do taśmy

            // Możesz dodać tutaj opóźnienie, aby procesy nie działały zbyt szybko (np. usleep(1000);)
        }
        // Zakończenie procesu potomnego
        exit(0);  // Proces kończy swoje działanie i nie idzie dalej
    }
    // Proces rodzicielski zwraca pid procesu potomnego
}

// Funkcja do zatrzymywania pracownika (ustawienie zmiennej running na false)
void Worker::stop() {
    running = false;
}



