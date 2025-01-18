#pragma once
#include <thread>
#include <atomic>
#include <iostream>
#include "Tasma.hpp"
#include "Dyspozytor.hpp"
#include <semaphore.h>
class Dyspozytor;

class Ciezarowka {
public:
    // Konstruktor
    Ciezarowka(int ladownosc, int id, Tasma& tasma, Dyspozytor& dyspozytor);
    
    // Destruktor
    ~Ciezarowka();
    
    // Rozpoczęcie pracy ciężarówki
    void start();
    
    // Zakończenie pracy ciężarówki
    void stop();
    
    // Funkcja uruchamiana w wątku
    void load();

    // Getter ID ciężarówki
    int getID() const;

    void czekaj();

    void powiadom();
    void zacznij();

    bool isReady();
    bool sprawdzStan(Dyspozytor& dyspozytor);
    std::thread& getThread() { return thread_; } 

private:
    int ladownosc_;         // Ładowność ciężarówki
    int id_;                // ID ciężarówki
    Tasma& tasma_;          // Referencja do taśmy
    Dyspozytor& dyspozytor_;
    std::thread thread_;    // Wątek ciężarówki
    std::atomic<bool> running_; // Flaga kontrolująca działanie wątku
    mutable std::mutex mtx_; // Mutex do synchronizacji
    std::condition_variable cv_;  // Zmienna warunkowa do oczekiwania na swoją kolej
    bool ready_to_load_ = false;  
};
