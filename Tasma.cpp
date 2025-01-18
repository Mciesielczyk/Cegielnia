// Tasma.cpp
#include "Tasma.hpp"
#include <iostream>
//std::mutex mtxx;
// Konstruktor
Tasma::Tasma(int maks_liczba_cegiel, int maks_masa)
    : maks_liczba_cegiel_(maks_liczba_cegiel), maks_masa_(maks_masa), aktualna_masa_(0) {}

// Dodawanie cegły na taśmę
bool Tasma::dodaj_cegle(int masa_cegly) {
    std::unique_lock<std::mutex> lock(mtx_);

    // Sprawdzanie, czy jest miejsce na taśmie i czy masa nie zostanie przekroczona
    if (tasma_.size() < maks_liczba_cegiel_ && (aktualna_masa_ + masa_cegly) <= maks_masa_) {
        tasma_.push(masa_cegly); // Dodanie cegły na taśmę
        aktualna_masa_ += masa_cegly;
        std::string message = "Dodano cegłę o masie " + std::to_string(masa_cegly) + ". Aktualna masa: " + std::to_string(aktualna_masa_) + ". Ilość cegieł: " + std::to_string(tasma_.size());
        
         {
            std::lock_guard<std::mutex> lock(cout_mutex); 
            std::cout << "\x1b[32m" << message << "\x1b[0m" << std::endl;
         }

        cv_.notify_one(); // Powiadomienie, że jest cegła do pobrania
        return true;
    } else {
        // Taśma jest pełna lub masa zostanie przekroczona
        return false;
    }
}

// Pobieranie cegły z taśmy
int Tasma::pobierz_cegle() {
    std::unique_lock<std::mutex> lock(mtx_);

    // Czekaj, aż na taśmie pojawi się cegła
    cv_.wait_for(lock,std::chrono::milliseconds(500),[this]{return !tasma_.empty();});

    // Pobranie cegły
    int masa_cegly = tasma_.front();
    if(masa_cegly!=0){
    tasma_.pop();
    aktualna_masa_ -= masa_cegly;
    
    std::string message1 = "Pobrano cegłę o masie " + std::to_string(masa_cegly) + ". Pozostała masa na taśmie: " + std::to_string(aktualna_masa_);
    
     {
        std::lock_guard<std::mutex> lock(cout_mutex); 
        std::cout << "\x1b[32m" << message1 << "\x1b[0m" << std::endl;
     }

    }
    return masa_cegly;
}
int Tasma::sprawdz_cegle() {
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait_for(lock,std::chrono::milliseconds(500),[this]{return !tasma_.empty();});
   
    int masa_cegly = tasma_.front();
    return masa_cegly;
}
// Sprawdzenie, czy taśma jest pusta
bool Tasma::czy_pusta() const {
    return tasma_.empty();
}
