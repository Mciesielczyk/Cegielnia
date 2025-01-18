// Tasma.hpp
#ifndef TASMA_HPP
#define TASMA_HPP

#include <mutex>
#include <queue>
#include <condition_variable>
#include "Mutex.hpp"

class Tasma {
public:
    // Konstruktor
    Tasma(int maks_liczba_cegiel, int maks_masa);

    // Dodawanie cegły na taśmę (zwraca true, jeśli się udało)
    bool dodaj_cegle(int masa_cegly);

    // Pobieranie cegły z taśmy (na potrzeby ciężarówki)
    int pobierz_cegle();
    int sprawdz_cegle();
    // Sprawdzenie, czy taśma jest pusta
    bool czy_pusta() const;

private:
    int maks_liczba_cegiel_; // Maksymalna liczba cegieł na taśmie
    int maks_masa_;          // Maksymalna masa taśmy
    int aktualna_masa_;      // Obecna masa cegieł na taśmie

    std::queue<int> tasma_;  // Kolejka reprezentująca cegły na taśmie (ich masy)
    mutable std::mutex mtx_; // Mutex do synchronizacji
    std::condition_variable cv_; // Zmienna warunkowa do zarządzania taśmą
};

#endif // TASMA_HPP
