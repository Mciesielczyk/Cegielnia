#include "Ciezarowka.hpp"
#include <chrono>
#include <iostream>
#include "Tasma.hpp"
#include "Dyspozytor.hpp"

// Konstruktor
std::mutex mtxi;
sem_t load_semaphore_;  


Ciezarowka::Ciezarowka(int ladownosc, int id, Tasma& tasma, Dyspozytor& dyspozytor)
    : ladownosc_(ladownosc), id_(id), tasma_(tasma), dyspozytor_ (dyspozytor), running_(false),ready_to_load_(false){
        sem_init(&load_semaphore_, 0, 1);
    }

// Destruktor
Ciezarowka::~Ciezarowka() {
    stop();
    if (thread_.joinable()) {
        thread_.join();
    }
    sem_destroy(&load_semaphore_);
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

  while(true){
        int current_load = 0;
        sem_wait(&load_semaphore_); 
    while (running_) {
        ready_to_load_=true;
        // Pobierz masę cegły z taśmy
        int masa_cegly = tasma_.sprawdz_cegle();
        
        std::string wiadomosc = "Ciężarówka " + std::to_string(id_) + " sprawdza teoretyczną cegłę o masie: " + std::to_string(masa_cegly);
         {
            std::lock_guard<std::mutex> lock(cout_mutex); 
            std::cout << wiadomosc<<std::endl;
         }

        // Sprawdzamy, czy ładunek nie przekroczy ładowności
        if (current_load + masa_cegly > ladownosc_) {
            //current_load = 0;  // Przerywamy, bo ciężarówka nie zmieści więcej cegieł
            running_=false;
        } else {
            // Pobieramy cegłę z taśmy
            masa_cegly = tasma_.pobierz_cegle();
            current_load += masa_cegly;
            if(masa_cegly!=0){
                 {
            std::lock_guard<std::mutex> lock(cout_mutex); 
            std::cout << "Ciężarówka " << id_ << " załadowała cegłę o masie " << masa_cegly << ". "
                      << "Aktualny ładunek: " << current_load << "/" << ladownosc_ << "\n";
                 }
            }
        }

        // Symulacja czasu załadunku cegły
        std::this_thread::sleep_for(std::chrono::milliseconds(70));  // Czas załadunku cegły
       // std::cout<<dyspozytor_.getCzyZatrzymal()<<" "<<tasma_.czy_pusta()<<std::endl;

        if(dyspozytor_.getCzyZatrzymal() && tasma_.czy_pusta() ){

                sem_post(&load_semaphore_);  // Podnosimy semafor, pozwalając innym wątkom kontynuować
                if(current_load!=0) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));  //czekamy az dowiezie
                    std::cout<<"Ostatnia ciężarówka "<<id_<<" dowiozła cegły."<<std::endl;
                }    
                else {
                        std::cout << "Ciężarówka " << id_ << " konczy zywot "<<std::endl;
                }
                stop();
                return;
        }
    }
         {

            std::lock_guard<std::mutex> lock(cout_mutex); 
            std::cout << "Ciężarówka " << id_ << " jest gotowa do odjazdu. "<< "Aktualny ładunek: " << current_load << "\n";
                      
         }
    ready_to_load_=false;
    sem_post(&load_semaphore_);  // Podnosimy semafor, pozwalając innym wątkom kontynuować
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));  // Czas rozladunku cegły
    running_=true;
    
    }
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