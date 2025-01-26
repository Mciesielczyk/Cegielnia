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
    /**
     * Konstruktor klasy Ciezarowka.
     * Inicjalizuje wszystkie parametry, takie jak ładowność, identyfikator, powiązaną taśmę
     * oraz dyspozytora, i ustawia semafor do synchronizacji.
     * 
     * ladownosc Maksymalna ładowność ciężarówki.
     * id Unikalny identyfikator ciężarówki.
     * tasma Obiekt taśmy, z którego ciężarówka pobiera cegły.
     * dyspozytor Obiekt dyspozytora zarządzający pracą ciężarówek.
     */
    Ciezarowka(int ladownosc, int id, Tasma& tasma, Dyspozytor& dyspozytor);
    
    /**
     * Destruktor klasy Ciezarowka.
     * Zatrzymuje wątek i usuwa semafor.
     */
    ~Ciezarowka();
    
    /**
     * Funkcja startująca wątek, który wykonuje załadunek cegieł na ciężarówkę.
     * Tworzy wątek, który wywołuje metodę `load()`.
     */
    void start();
    
    /**
     * Funkcja zatrzymująca wątek ciężarówki.
     * Zmienia flagę `running_` na `false`, co zatrzymuje wykonywanie wątku.
     */
    void stop();
    
    /**
     * Funkcja odpowiedzialna za załadunek cegieł na ciężarówkę.
     * Ciężarówka sprawdza masę cegieł i ładuje je, dopóki nie osiągnie maksymalnej ładowności.
     * Używa semafora, aby zsynchronizować dostęp do zasobów.
     */
    void load();

    
      //Funkcja zwracająca unikalny identyfikator ciężarówki.
     
    int getID() const;

    /**
     * Funkcja sprawdzająca, czy ciężarówka jest gotowa do załadunku.
     * True, jeśli ciężarówka jest gotowa do załadunku, false w przeciwnym razie.
     */
    bool isReady();

    
      //Funkcja sprawdzająca stan dyspozytora, określa, czy należy zakończyć pracę.
      //True, jeśli dyspozytor zatrzymał proces, w przeciwnym razie false.
    
    bool sprawdzStan(Dyspozytor& dyspozytor);



private:
    int ladownosc_;        // Maksymalna ładowność ciężarówki
    int id_;                // Identyfikator ciężarówki
    Tasma& tasma_;          // Referencja do obiektu taśmy
    Dyspozytor& dyspozytor_; // Referencja do obiektu dyspozytora
    std::thread thread_;    // Wątek ciężarówki
    std::atomic<bool> running_;  // Flaga kontrolująca stan wątku
    bool ready_to_load_ = false; // Flaga wskazująca, czy ciężarówka jest gotowa do załadunku
};
