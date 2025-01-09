#include "Dyspozytor.hpp"
#include <chrono>
#include <thread>

Dyspozytor::Dyspozytor(int liczba_ciezarowek, int ladownosc, int czas)
    : liczba_ciezarowek_(liczba_ciezarowek), ladownosc_(ladownosc), czas_(czas) {

    for (int i = 0; i < liczba_ciezarowek_; ++i) {
        ciezarowki_.emplace_back(ladownosc_, i + 1);
    }
}

void Dyspozytor::rozpocznijPrace() {
    for (auto& ciezarowka : ciezarowki_) {
        ciezarowka.rozpocznijTransport();
    }
}

void Dyspozytor::wyslijSygnal1() {
    // Sygnał 1 - pozwól ciężarówce odjechać z niepełnym ładunkiem
    for (auto& ciezarowka : ciezarowki_) {
        if (ciezarowka.getAktualnaMasa() < ladownosc_) {
            ciezarowka.zatrzymajTransport();
            std::cout << "Ciezarowka " << ciezarowka.getNumer() << " odjechała z niepełnym ładunkiem." << std::endl;
        }
    }
}

void Dyspozytor::wyslijSygnal2() {
    // Sygnał 2 - zakończ pracę
    std::cout << "Dyspozytor zakończył pracę!" << std::endl;
    for (auto& ciezarowka : ciezarowki_) {
        ciezarowka.zatrzymajTransport();
    }
}