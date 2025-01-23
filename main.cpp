#include "worker.h"
#include "conveyor_belt.h"
#include <unistd.h>

int main() {


    ConveyorBelt* belt = initConveyorBelt();

    // Tworzymy pracowników (procesy), którzy będą dodawać cegły do taśmy
    Worker worker1(*belt, 1);  // Pracownik dodający cegłę o wadze 1
    Worker worker2(*belt, 2);  // Pracownik dodający cegłę o wadze 2
    Worker worker3(*belt, 3);  // Pracownik dodający cegłę o wadze 3

    worker1.start();  // Uruchamia pierwszego pracownika
    worker2.start();  // Uruchamia drugiego pracownika
    worker3.start();  // Uruchamia trzeciego pracownika

    // Pracownicy dodają cegły przez chwilę...
    // Możesz np. poczekać, dając im czas na wykonanie zadania
    sleep(1);  // Czekaj przez 1 sekundę

    // Zatrzymanie pracowników
    worker1.stop();
    worker2.stop();
    worker3.stop();

    // Możesz teraz sprawdzić stan taśmy
    printBelt(belt);


    return 0;
}

