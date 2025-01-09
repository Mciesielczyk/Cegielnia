// Tasma.cpp
#include "Tasma.hpp"
#include <iostream>

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
        std::cout << "Dodano cegłę o masie " << masa_cegly << ". Aktualna masa: " << aktualna_masa_ << ". Ilosc cegiel: "<<tasma_.size()<< "\n";
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
    cv_.wait(lock, [this] { return !tasma_.empty(); });

    // Pobranie cegły
    int masa_cegly = tasma_.front();
    tasma_.pop();
    aktualna_masa_ -= masa_cegly;

    std::cout << "Pobrano cegłę o masie " << masa_cegly << ". Pozostała masa: " << aktualna_masa_ << "\n";
    return masa_cegly;
}
int Tasma::sprawdz_cegle() {
    int masa_cegly = tasma_.front();
    return masa_cegly;
}
// Sprawdzenie, czy taśma jest pusta
bool Tasma::czy_pusta() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return tasma_.empty();
}
