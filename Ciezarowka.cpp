#include "Ciezarowka.hpp"
#include <chrono>
#include <iostream>
#include "Tasma.hpp"
#include "Dyspozytor.hpp"
// Konstruktor
std::mutex mtxi;
Ciezarowka::Ciezarowka(int ladownosc, int id, Tasma& tasma, Dyspozytor& dyspozytor)
    : ladownosc_(ladownosc), id_(id), tasma_(tasma), dyspozytor_ (dyspozytor), running_(false),ready_to_load_(false){}

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
    //std::cout<<"Ciezarowka dostała sygnał"<<std::endl;
}

// Funkcja uruchamiana w wątku - załadunek cegieł
void Ciezarowka::load() {
    int current_load = 0;
    while(true){
        current_load = 0;
    while (running_) {
        std::unique_lock<std::mutex> lock(mtxi);  // Zablokowanie mutexu na czas działania tej funkcji
        ready_to_load_=true;
        // Pobierz masę cegły z taśmy
        int masa_cegly = tasma_.sprawdz_cegle();
        std::cout << "Ciężarówka " << id_ << " sprawdza teoretyczna cegle o masie: " << masa_cegly << std::endl;

        // Sprawdzamy, czy ładunek nie przekroczy ładowności
        if (current_load + masa_cegly > ladownosc_) {
            //current_load = 0;  // Przerywamy, bo ciężarówka nie zmieści więcej cegieł
            running_=false;
        } else {
            // Pobieramy cegłę z taśmy
            masa_cegly = tasma_.pobierz_cegle();
            current_load += masa_cegly;
            if(masa_cegly!=0){
            std::cout << "Ciężarówka " << id_ << " załadowała cegłę o masie " << masa_cegly << ". "
                      << "Aktualny ładunek: " << current_load << "/" << ladownosc_ << "\n";
            }
        }

        // Symulacja czasu załadunku cegły
        std::this_thread::sleep_for(std::chrono::milliseconds(40));  // Czas załadunku cegły
       // std::cout<<dyspozytor_.getCzyZatrzymal()<<" "<<tasma_.czy_pusta()<<std::endl;

        if(dyspozytor_.getCzyZatrzymal() && tasma_.czy_pusta() ){
                std::cout << "Ciężarówka " << id_ << " konczy zywot "<<std::endl;
                if(current_load!=0)     std::this_thread::sleep_for(std::chrono::milliseconds(1000));  //czekamy az dowiezie
                stop();
                lock.unlock();
                return;
        }
    }
    std::cout << "Ciężarówka " << id_ << " jest gotowa do odjazdu. "
                      << "Aktualny ładunek: " << current_load << "\n";
    ready_to_load_=false;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // Czas załadunku cegły
    running_=true;
    }
    //powiadom();  // Powiadomienie drugiej ciężarówki
    std::cout << "Ciężarówka " << id_ << " zakończyła pracę.\n";
}


// Getter ID ciężarówki
int Ciezarowka::getID() const {
    return id_;
}




bool Ciezarowka::isReady(){
    return ready_to_load_;
}

bool Ciezarowka::sprawdzStan(Dyspozytor& dyspozytor){
    bool czy=dyspozytor.getCzyZatrzymal();
    return czy;
}