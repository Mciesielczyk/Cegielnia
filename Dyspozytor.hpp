// Dyspozytor.hpp
#pragma once
#include <vector>
#include <iostream>
#include "Ciezarowka.hpp"

class Dyspozytor {
public:
    Dyspozytor(int liczba_ciezarowek, int ladownosc, int czas);
    void rozpocznijPrace(); // Rozpocznij pracę dyspozytora
    void wyslijSygnal1();   // Wysłań sygnał 1 - pozwól ciężarówce odjechać
    void wyslijSygnal2();   // Wysłań sygnał 2 - zakończ pracę

private:
    int liczba_ciezarowek_;  // Liczba ciężarówek
    int ladownosc_;          // Ładowność ciężarówki
    int czas_;               // Czas pracy
    std::vector<Ciezarowka> ciezarowki_; // Lista ciężarówek
};
