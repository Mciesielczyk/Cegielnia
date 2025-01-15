#pragma once
#include <thread>
#include <atomic>
#include <iostream>
#include "Tasma.hpp"

class Ciezarowka {
public:
    // Konstruktor
    Ciezarowka(int ladownosc, int id, Tasma& tasma);
    
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
private:
    int ladownosc_;         // Ładowność ciężarówki
    int id_;                // ID ciężarówki
    Tasma& tasma_;          // Referencja do taśmy
    std::thread thread_;    // Wątek ciężarówki
    std::atomic<bool> running_; // Flaga kontrolująca działanie wątku
    mutable std::mutex mtx_; // Mutex do synchronizacji
    
    std::condition_variable cv_;  // Zmienna warunkowa do oczekiwania na swoją kolej
    bool ready_to_load_ = false;  
};
