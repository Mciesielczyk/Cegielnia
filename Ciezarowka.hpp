#pragma once
#include <thread>
#include <atomic>
#include <iostream>
#include "Tasma.hpp"
#include "Dyspozytor.hpp"
#include <semaphore.h>

#define CZAS_ZALADUNKU 100
#define CZAS_ROZLADUNKU 1500

class Dyspozytor;

class Ciezarowka {
public:
    Ciezarowka(int ladownosc, int id, Tasma& tasma, Dyspozytor& dyspozytor);
    
    ~Ciezarowka();
    
    void start();
    
    void stop();
    
    void load();

    int getID() const;

    bool isReady();

    bool sprawdzStan(Dyspozytor& dyspozytor);

    std::thread& getThread() { return thread_; } 

private:
    int ladownosc_;        
    int id_;                
    Tasma& tasma_;          
    Dyspozytor& dyspozytor_;
    std::thread thread_;    
    std::atomic<bool> running_; 
    bool ready_to_load_ = false;  
};
