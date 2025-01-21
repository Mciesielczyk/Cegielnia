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

    void startPracownikow();  
    void startCiezarowek();
    void sygnal2();   // Zatrzymanie wszystkich pracowników
    void sygnal1();       // wyslanie ciezarowki
    bool getCzyZatrzymal();
private:
    std::vector<std::unique_ptr<Pracownik>> pracownicy_;  
    std::vector<std::unique_ptr<Ciezarowka>> ciezarowki_;  
    
    std::mutex mtx_;  
    std::condition_variable cv_;  
    Tasma& tasma_;
    bool czy_zatrzymal=false;

};

#endif // DYSPOSYTOR_HPP
