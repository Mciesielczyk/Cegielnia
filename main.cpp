#include <iostream>
#include <thread>
#include <chrono>
#include "Pracownik.hpp"
#include "Tasma.hpp"
#include "Ciezarowka.hpp"
#include "Dyspozytor.hpp"


int main() {
    // Parametry taśmy
    int maks_liczba_cegiel = 10; // Maksymalna liczba cegieł
    int maks_masa = 15;          // Maksymalna masa
    int ladownosc_ciezarowki = 15; // Ładowność każdej ciężarówki
    int liczba_ciezarowek = 5;    // Liczba dostępnych ciężarówek
    // Inicjalizacja taśmy
    Tasma tasma(maks_liczba_cegiel, maks_masa);

    // Inicjalizacja pracowników
    //Pracownik p1(1, 1, tasma);
    //Pracownik p2(2, 2, tasma);
    //Pracownik p3(3, 3, tasma);
    Dyspozytor dyspozytor(tasma);
    dyspozytor.startPracownikow();
    dyspozytor.startCiezarowek();
    //Ciezarowka c1(15,1,tasma);
    // Start pracy pracowni

    //c1.start();

    // Symulacja pracy przez 10 sekund
    std::this_thread::sleep_for(std::chrono::seconds(10));
    // Symulacja pracy przez 10 sekund

    // Zatrzymanie pracy
    dyspozytor.sygnal2();
    //c1.stop();
    return 0;
}
