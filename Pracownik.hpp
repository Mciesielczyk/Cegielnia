
#ifndef PRACOWNIK_HPP
#define PRACOWNIK_HPP

#include <thread>
#include <atomic>
#include "Tasma.hpp"

class Pracownik {
public:
    Pracownik(int id, int masa_cegly, Tasma& tasma);

    ~Pracownik();

    void start();

    void stop();

    int getID() const;

    std::thread& getThread() { return thread_; } 
private:
    // Funkcja wykonywana w wątku
    void produce();

    int id_;                     
    int masa_cegly_;             
    Tasma& tasma_;              
    std::thread thread_;        
    std::atomic<bool> running_;  // atomowa, ze inne watki nie maja dostepu do niej

};

#endif 
