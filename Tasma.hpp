#ifndef TASMA_HPP
#define TASMA_HPP

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <string>

// Definicja nazwy pamięci współdzielonej
#define SHM_NAME "/my_shm_tasma"

// Struktura reprezentująca pamięć współdzieloną (tablica cegieł + zmienne)
struct SharedMemory {
    int tasma[100];                // Tablica przechowująca cegły (można dynamicznie dostosować rozmiar)
    int front;                     // Indeks frontu
    int rear;                      // Indeks rear
    int count;                     // Liczba cegieł
    int aktualna_masa;             // Aktualna masa na taśmie
};

class Tasma {
public:
    // Konstruktor i destruktor
    Tasma(int maks_liczba_cegiel, int maks_masa);
    ~Tasma();

    // Metody do obsługi taśmy
    bool dodaj_cegle(int masa_cegly);
    int pobierz_cegle();
    int sprawdz_cegle();
    bool czy_pusta() const;
    bool initSharedResources();
    void cleanupSharedResources();
     int shmID;
        int semID;

private:
    int maks_liczba_cegiel_;   // Maksymalna liczba cegieł na taśmie
    int maks_masa_;            // Maksymalna masa taśmy

    // Wskaźnik do pamięci współdzielonej (tablica cegieł)
    int* tasma_;  

    // Atomiczne zmienne dla indeksów i liczby cegieł
    std::atomic<int> front_;
    std::atomic<int> rear_;
    std::atomic<int> count_;
    std::atomic<int> aktualna_masa_;  // Użycie std::atomic dla tej zmiennej

    // Synchronizacja dostępu do danych (mutex i condition variable)
    mutable std::mutex mtx_; 
    std::condition_variable cv_; 
    
    // Deskriptor pamięci współdzielonej
    int shm_fd_; 
};

#endif // TASMA_HPP
