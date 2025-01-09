#include "Ciezarowka.hpp"
#include <chrono>
#include <iostream>
#include "Tasma.hpp"
// Konstruktor
Ciezarowka::Ciezarowka(int ladownosc, int id, Tasma& tasma)
    : ladownosc_(ladownosc), id_(id), tasma_(tasma), running_(false) {}

// Destruktor
Ciezarowka::~Ciezarowka() {
    stop();
    if (thread_.joinable()) {
        thread_.join();
    }
}

// Rozpoczęcie pracy ciężarówki
void Ciezarowka::start() {
    running_ = true;
    thread_ = std::thread(&Ciezarowka::load, this);
    std::cout << "Ciężarówka " << id_ << " rozpoczęła prace.\n";
}

// Zakończenie pracy ciężarówki
void Ciezarowka::stop() {
    running_ = false;
}

// Funkcja uruchamiana w wątku - załadunek cegieł
void Ciezarowka::load() {
    while(true){
            int current_load = 0;
      while (running_ && !tasma_.czy_pusta()) {
        // Czekamy na dostępność cegły na taśmie
        int masa_cegly=tasma_.sprawdz_cegle();

        if(current_load+masa_cegly>ladownosc_){
            std::cout << "Ciężarówka " << id_ << " jest pełna i gotowa do odjazdu.\n";
            break;
        }

        masa_cegly = tasma_.pobierz_cegle();

        // Dodajemy cegłę do ciężarówki
        current_load += masa_cegly;
        std::cout << "Ciężarówka " << id_ << " załadowała cegłę o masie " << masa_cegly << ". "
                  << "Aktualny ładunek: " << current_load << "/" << ladownosc_ << "\n";
        
        if (current_load >= ladownosc_) {
            std::cout << "Ciężarówka " << id_ << " jest pełna i gotowa do odjazdu.\n";
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Symulacja czasu załadunku
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(600)); // Symulacja czasu załadunku

}
}

// Getter ID ciężarówki
int Ciezarowka::getID() const {
    return id_;
}
