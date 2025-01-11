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
    std::unique_lock<std::mutex> lock(mtx_);
    int current_load = 0;
    
    while (running_) {
        // Sprawdzamy masę cegły, która jest dostępna na taśmie
        int masa_cegly = tasma_.sprawdz_cegle();
        std::cout<<masa_cegly;
        // Jeśli dodanie cegły do aktualnego ładunku przekroczy ładowność, to nie dodajemy cegły
        if (current_load + masa_cegly > ladownosc_) {
            std::cout << "Ciężarówka " << id_ << " jest pełna i gotowa do odjazdu. "
                      << "Wartość załadunku: " << current_load << "\n";
            
            // Symulacja czasu jazdy
            std::this_thread::sleep_for(std::chrono::milliseconds(600));
            
            // Resetowanie ładunku po przejeździe
            current_load = 0;
        } else {
            // Jeśli ładunek nie przekroczy ładowności, pobieramy cegłę
            masa_cegly = tasma_.pobierz_cegle();
            current_load += masa_cegly;

            std::cout << "Ciężarówka " << id_ << " załadowała cegłę o masie " << masa_cegly << ". "
                      << "Aktualny ładunek: " << current_load << "/" << ladownosc_ << "\n";
        }

        // Symulacja czasu załadunku
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }

    std::cout << "Ciężarówka " << id_ << " zakończyła pracę.\n";
}



// Getter ID ciężarówki
int Ciezarowka::getID() const {
    return id_;
}
