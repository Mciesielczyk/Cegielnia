#ifndef TASMA_HPP
#define TASMA_HPP

#include <mutex>
#include <queue>
#include <condition_variable>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>  // Dla pid_t
#include <sys/wait.h>   // Dla waitpid
#include <unistd.h>     // Dla fork i kill
#include <csignal>      // Dla obsługi sygnałów, np. kill
#include <thread> // Dla std::this_thread::sleep_for
#include <semaphore.h>  // Nagłówek dla semaforów POSIX


class Tasma {
public:
    // Konstruktor klasy, który ustawia maksymalne wartości dla liczby cegieł i masy
    Tasma(int maks_liczba_cegiel, int maks_masa);

    // Destruktor klasy, który zajmuje się czyszczeniem zasobów
    ~Tasma();

    // Funkcja dodająca cegłę na taśmę. Zwraca true, jeśli udało się dodać cegłę,
    // false w przypadku gdy taśma jest pełna lub masa przekroczyła dopuszczalny limit
    bool dodaj_cegle(int masa_cegly);

    // Funkcja, która pobiera cegłę z taśmy, na potrzeby ciężarówki
    // Zwraca masę cegły lub -1, jeśli taśma jest pusta
    int pobierz_cegle();

    // Funkcja sprawdzająca, czy taśma zawiera cegły. Zwraca liczbę cegieł na taśmie.
    int sprawdz_cegle();

    // Funkcja sprawdzająca, czy taśma jest pusta 
    bool czy_pusta() const;

    // Funkcja wyświetlająca stan kolejki cegieł 
    void debugKolejka();

private:
    // Maksymalne ograniczenia taśmy (liczba cegieł i masa)
    int maks_liczba_cegiel_;
    int maks_masa_;

    // Semafory POSIX do synchronizacji
    sem_t* sem_mutex_;            // Semafor mutex (do wzajemnego wykluczania, chroniący przed równoczesnym dostępem)
    sem_t* sem_space_available_;  // Semafor wskazujący, czy jest dostępne miejsce na taśmie
    sem_t* sem_items_available_;  // Semafor wskazujący, czy na taśmie są dostępne cegły

    // Pamięć dzielona, do której zapisują i z której odczytują różne procesy
    const char* shm_name_ = "/shared_tasma";  // Nazwa segmentu pamięci dzielonej
    int shm_fd_;                             // Deskryptor pliku pamięci dzielonej
    size_t shm_size_;                        // Rozmiar segmentu pamięci dzielonej
    struct SharedQueue {
        int data[100];      // Tablica przechowująca cegły w postaci masy
        int head;            // Wskaźnik na początek kolejki
        int tail;            // Wskaźnik na koniec kolejki
        int aktualna_masa_;  // Bieżąca masa cegieł na taśmie
    } *shared_queue_;    // Wskaźnik na strukturę pamięci dzielonej

   
    void utworzPamiecDzielona();  // Funkcja tworząca segment pamięci dzielonej
    void zwolnijPamiecDzielona(); // Funkcja zwalniająca segment pamięci dzielonej
};


#endif