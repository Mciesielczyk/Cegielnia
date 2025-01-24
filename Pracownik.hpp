#ifndef PRACOWNIK_HPP
#define PRACOWNIK_HPP

#include "Tasma.hpp"
#include <atomic>
#include <sys/types.h> // Dla pid_t

class Pracownik {
public:
    Pracownik(int id, int masa_cegly, Tasma& tasma);

    ~Pracownik();

    void start();   // Uruchomienie procesu

    void stop();    // Zatrzymanie procesu

    int getID() const; // Pobierz ID pracownika

private:
    void produce(); // Funkcja wykonywana w procesie potomnym

    int id_;                     // Identyfikator pracownika
    int masa_cegly_;             // Masa cegły, którą dodaje
    Tasma& tasma_;               // Referencja do taśmy
    pid_t pid_;                  // Identyfikator procesu potomnego
    std::atomic<bool> running_;  // Flaga kontrolująca stan pracy
};

#endif
