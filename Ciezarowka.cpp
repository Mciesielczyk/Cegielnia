#include "Ciezarowka.hpp"
#include <chrono>
#include <iostream>
#include "Tasma.hpp"
// Konstruktor
std::mutex mtxi;
Ciezarowka::Ciezarowka(int ladownosc, int id, Tasma& tasma)
    : ladownosc_(ladownosc), id_(id), tasma_(tasma), running_(false),ready_to_load_(false){}

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
    int current_load = 0;
    while(true){
    while (running_) {
        std::lock_guard<std::mutex> lock(mtxi);  // Zablokowanie mutexu na czas działania tej funkcji

        // Pobierz masę cegły z taśmy
        int masa_cegly = tasma_.sprawdz_cegle();
        std::cout << "Ciężarówka " << id_ << " sprawdza cegłę o masie: " << masa_cegly << std::endl;

        // Sprawdzamy, czy ładunek nie przekroczy ładowności
        if (current_load + masa_cegly > ladownosc_) {
            std::cout << "Ciężarówka " << id_ << " jest pełna i gotowa do odjazdu. "
                      << "Aktualny ładunek: " << current_load << "\n";
            current_load = 0;  // Przerywamy, bo ciężarówka nie zmieści więcej cegieł
            break;
        } else {
            // Pobieramy cegłę z taśmy
            masa_cegly = tasma_.pobierz_cegle();
            current_load += masa_cegly;
            std::cout << "Ciężarówka " << id_ << " załadowała cegłę o masie " << masa_cegly << ". "
                      << "Aktualny ładunek: " << current_load << "/" << ladownosc_ << "\n";
        }

        // Symulacja czasu załadunku cegły
        std::this_thread::sleep_for(std::chrono::milliseconds(40));  // Czas załadunku cegły
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(400));  // Czas załadunku cegły
    running_=true;
    }
    //powiadom();  // Powiadomienie drugiej ciężarówki
    std::cout << "Ciężarówka " << id_ << " zakończyła pracę.\n";
}


// Getter ID ciężarówki
int Ciezarowka::getID() const {
    return id_;
}


void Ciezarowka::czekaj() {
    std::unique_lock<std::mutex> lock(mtxi);
    std::cout << "Ciężarówka " << id_ << " czeka na powiadomienie...\n";
    while (!ready_to_load_) {
        cv_.wait(lock);
    }
    std::cout << "Ciężarówka " << id_ << " otrzymała powiadomienie!\n";
    ready_to_load_ = false;  // Ustawiamy na false, aby wątek czekał na kolejne powiadomienie
}

void Ciezarowka::powiadom() {
    std::unique_lock<std::mutex> lock(mtxi);
    ready_to_load_ = false;  // Po powiadomieniu ustawiamy gotowość na false
    std::cout << "Ciężarówka " << id_ << " powiadamia drugą ciężarówkę!\n";
    cv_.notify_one();  // Powiadamiamy drugą ciężarówkę
}


void Ciezarowka::zacznij(){
    std::unique_lock<std::mutex> lock(mtxi);
    ready_to_load_=true;
    cv_.notify_one(); 
}