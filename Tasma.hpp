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
    Tasma(int maks_liczba_cegiel, int maks_masa);
    ~Tasma();

    // Dodawanie cegły na taśmę (zwraca true, jeśli się udało)
    bool dodaj_cegle(int masa_cegly);

    // Pobieranie/sprawdzanie cegły z taśmy (na potrzeby ciężarówki)
    int pobierz_cegle();
    int sprawdz_cegle();
    bool czy_pusta() const;
    void debugKolejka();
private:
    // Maksymalne ograniczenia
    int maks_liczba_cegiel_;
    int maks_masa_;

    // Semafory
    sem_t* sem_mutex_;            // Semafor mutex (wzajemne wykluczanie)
    sem_t* sem_space_available_;  // Semafor dostępności miejsca
    sem_t* sem_items_available_;  // Semafor dostępności cegieł

    // Pamięć dzielona
    const char* shm_name_ = "/shared_tasma";
    int shm_fd_;
    size_t shm_size_;
    struct SharedQueue {
        int data[1000];
        int head;
        int tail;
        int aktualna_masa_;
    } *shared_queue_;

    // Funkcje pomocnicze
    void utworzPamiecDzielona();
    void zwolnijPamiecDzielona();
};

#endif
