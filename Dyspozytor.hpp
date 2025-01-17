// Dyspozytor.hpp
#ifndef DYSPOSYTOR_HPP
#define DYSPOSYTOR_HPP

#include <vector>
#include <memory>  // Include for std::unique_ptr
#include "Pracownik.hpp"
#include "Ciezarowka.hpp"

class Ciezarowka;

class Dyspozytor {
public:
    Dyspozytor(Tasma& tasma,  int liczba_ciezarowek, int ladownosc_ciezarowki);
    ~Dyspozytor();

    void startPracownikow();  // Uruchomienie wszystkich pracowników
    void startCiezarowek();
    void sygnal2();   // Zatrzymanie wszystkich pracowników
    void sygnal1();
    bool getCzyZatrzymal();
private:
    std::vector<std::unique_ptr<Pracownik>> pracownicy_;  // Use unique_ptr for non-copyable Pracownik objects
    std::vector<std::unique_ptr<Ciezarowka>> ciezarowki_;  // Use unique_ptr for non-copyable Pracownik objects
    
    std::mutex mtx_;  // Mutex do synchronizacji
    std::condition_variable cv_;  // Zmienna warunkowa
    Tasma& tasma_;
    bool czy_zatrzymal=false;

};

#endif // DYSPOSYTOR_HPP
