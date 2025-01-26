
#ifndef DYSPOSYTOR_HPP
#define DYSPOSYTOR_HPP

#include <vector>
#include <memory>  // Include for std::unique_ptr
#include "Pracownik.hpp"
#include "Ciezarowka.hpp"

class Ciezarowka;

class Dyspozytor {
public:
    Dyspozytor(Tasma& tasma, int liczba_ciezarowek, int ladownosc_ciezarowki);
    ~Dyspozytor();

    void startPracownikow();  // Uruchamia pracowników
    void startCiezarowek();   // Uruchamia ciężarówki
    void sygnal2();           // Zatrzymuje wszystkich pracowników
    void sygnal1();           // Wysyła sygnał do pierwszej gotowej ciężarówki
    bool getCzyZatrzymal();   // Sprawdza, czy proces został zatrzymany przez dyspozytora
private:
    std::vector<std::unique_ptr<Pracownik>> pracownicy_;  // Lista pracowników
    std::vector<std::unique_ptr<Ciezarowka>> ciezarowki_;  // Lista ciężarówek
    
    std::mutex mtx_;  // Mutex do synchronizacji
    std::condition_variable cv_;  // Zmienna warunkowa
    Tasma& tasma_;  // Referencja do taśmy
    bool czy_zatrzymal = false;  // Flaga zatrzymania procesu
};

#endif 