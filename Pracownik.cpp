// Pracownik.cpp
#include "Pracownik.hpp"
#include <chrono>
#include <iostream>

// Konstruktor
Pracownik::Pracownik(int id, int masa_cegly, Tasma& tasma)
    : id_(id), masa_cegly_(masa_cegly), tasma_(tasma), running_(false) {}

// Destruktor
Pracownik::~Pracownik() {
    stop();
    if (thread_.joinable()) {
        thread_.join();
    }
}

// Metoda uruchamiająca proces produkcji
void Pracownik::start() {
    running_ = true;
    thread_ = std::thread(&Pracownik::produce, this);
    std::string message2 = "Pracownik P" + std::to_string(id_) + " rozpoczął pracę.\n";
    std::cout << "\x1b[94m" << message2<< "\x1b[0m";

}

// Metoda zatrzymująca proces produkcji
void Pracownik::stop() {
    running_ = false;
}

// Getter ID pracownika
int Pracownik::getID() const {
    return id_;
}

// Funkcja wykonywana w wątku
void Pracownik::produce() {
    while (running_) {
        // Próba dodania cegły na taśmę
        
        bool dodano = tasma_.dodaj_cegle(masa_cegly_);

        if (dodano) {
            //std::cout << "Pracownik P" << id_ << " dodał cegłę o masie " << masa_cegly_ << ".\n";
        } else {
            std::string message = "Pracownik P" + std::to_string(id_) + " nie mógł dodać cegły - taśma pełna lub przekroczono masę.\n";
            std::cout << "\x1b[94m" << message<< "\x1b[0m";
        }

        // Symulacja czasu produkcji cegły
        std::this_thread::sleep_for(std::chrono::milliseconds(600)); // Dostosuj czas w razie potrzeby
    }
    std::string message1 = "Pracownik P" + std::to_string(id_) + " zakończył pracę.\n";
    std::cout << "\x1b[94m" << message1<< "\x1b[0m";

}
