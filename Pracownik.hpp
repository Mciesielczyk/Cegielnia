#ifndef PRACOWNIK_HPP
#define PRACOWNIK_HPP

#include "Tasma.hpp"
#include <atomic>               // Nagłówek dla typów atomowych, takich jak std::atomic, wykorzystywany do zapewnienia bezpiecznego współdzielenia zmiennych pomiędzy różnymi wątkami.
#include <sys/types.h>          // Nagłówek, który zawiera definicje podstawowych typów systemowych, takich jak pid_t (typ procesu), uid_t (typ identyfikatora użytkownika), itp.
#include <csignal>              // Nagłówek dla obsługi sygnałów w systemie, np. SIGINT (przerwanie), SIGTERM (zakończenie), SIGUSR1 (użytkownik zdefiniowany sygnał), itp.
#include <sys/prctl.h>          // Nagłówek dla prctl(), funkcji systemowej pozwalającej na kontrolowanie zachowania procesu, np. ustawienie jego nazwy w systemie.
#include <chrono>               // Nagłówek dla funkcji związanych z czasem i datą, takich jak std::chrono::seconds, std::chrono::milliseconds itp., umożliwiający operacje na czasie.
#include <iostream>             // Nagłówek umożliwiający korzystanie z funkcji do wejścia/wyjścia (np. std::cout, std::cin) do komunikacji z użytkownikiem.
#include <unistd.h>             // Nagłówek zawierający funkcje systemowe do pracy z procesami, takie jak getpid() (pobranie PID procesu) lub sleep() (spowolnienie działania procesu).


#define CZAS_PRACY_PRACOWNIKA 500

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
    int czas_pracy_pracownika;
    Tasma& tasma_;               // Referencja do taśmy
    pid_t pid_;                  // Identyfikator procesu potomnego
    std::atomic<bool> running_;  // Flaga kontrolująca stan pracy
};

#endif
