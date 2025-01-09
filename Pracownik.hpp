// Pracownik.hpp
#ifndef PRACOWNIK_HPP
#define PRACOWNIK_HPP

#include <thread>
#include <atomic>
#include "Tasma.hpp"

class Pracownik {
public:
    // Konstruktor
    Pracownik(int id, int masa_cegly, Tasma& tasma);

    // Destruktor
    ~Pracownik();

    // Metoda uruchamiająca proces produkcji
    void start();

    // Metoda zatrzymująca proces produkcji
    void stop();

    // Getter ID pracownika
    int getID() const;

private:
    // Funkcja wykonywana w wątku
    void produce();

    int id_;                     // ID pracownika (np. 1 dla P1)
    int masa_cegly_;             // Masa produkowanej cegły
    Tasma& tasma_;               // Referencja do taśmy transportowej
    std::thread thread_;         // Wątek pracownika
    std::atomic<bool> running_;  // Flaga kontrolująca działanie wątku
};

#endif // PRACOWNIK_HPP
