
#ifndef TASMA_HPP
#define TASMA_HPP

#include <mutex>
#include <queue>
#include <condition_variable>
#include "Mutex.hpp"

class Tasma {
public:
    Tasma(int maks_liczba_cegiel, int maks_masa);

    // Dodawanie cegły na taśmę (zwraca true, jeśli się udało)
    bool dodaj_cegle(int masa_cegly);

    // Pobieranie/sprawdzanie cegły z taśmy (na potrzeby ciężarówki)
    int pobierz_cegle();
    int sprawdz_cegle();
    bool czy_pusta() const;

private:
    int maks_liczba_cegiel_; 
    int maks_masa_;         
    int aktualna_masa_;     

    std::queue<int> tasma_;  // Kolejka reprezentująca cegły na taśmie (ich masy)
    mutable std::mutex mtx_; 
    std::condition_variable cv_; // Zmienna warunkowa do zarządzania taśmą
};

#endif 
