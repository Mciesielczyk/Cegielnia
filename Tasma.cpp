#include "Tasma.hpp"
#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h> 

Tasma::Tasma(int maks_liczba_cegiel, int maks_masa)
    : maks_liczba_cegiel_(maks_liczba_cegiel), maks_masa_(maks_masa), aktualna_masa_(0) {

    // Tworzenie pamięci współdzielonej
    int fd = -1;
    if ((fd = open(SHM_NAME, O_CREAT | O_RDWR, 0666)) == -1) {
        std::cerr << "Unable to open shared memory file." << std::endl;
        return;
    }

    // Ustawienie rozmiaru pamięci współdzielonej (tablica cegieł + zmienne front, rear, count, aktualna_masa)
    size_t size = sizeof(int) * maks_liczba_cegiel + sizeof(std::atomic<int>) * 4;  // Tablica + front, rear, count, aktualna_masa
    if (ftruncate(fd, size) == -1) {
        std::cerr << "Unable to set size of shared memory." << std::endl;
        close(fd);
        return;
    }

    // Mapowanie pamięci współdzielonej
    SharedMemory* shared_mem = (SharedMemory*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_mem == MAP_FAILED) {
        std::cerr << "Unable to map shared memory." << std::endl;
        close(fd);
        return;
    }

    // Inicjalizacja pamięci współdzielonej
    //shared_mem->tasma = (int*)shared_mem + 4;  // Offset dla tablicy cegieł
    memset(shared_mem->tasma, 0, sizeof(int) * maks_liczba_cegiel);  // Zerowanie tablicy cegieł

    // Inicjalizacja atomowych zmiennych
    shared_mem->front = 0;
    shared_mem->rear = 0;
    shared_mem->count = 0;
    shared_mem->aktualna_masa = 0;

    // Zamknięcie pliku po zakończeniu pracy z pamięcią współdzieloną
    close(fd);

    // Użycie pamięci współdzielonej w klasie
    tasma_ = shared_mem->tasma;
//front_.store(shared_mem->front.load());
//rear_.store(shared_mem->rear.load());
//count_.store(shared_mem->count.load());
//aktualna_masa_.store(shared_mem->aktualna_masa.load());

}

Tasma::~Tasma() {
    // Zwolnienie pamięci współdzielonej
    if (munmap(tasma_, sizeof(int) * maks_liczba_cegiel_ + sizeof(std::atomic<int>) * 4) == -1) {
        std::cerr << "Unable to unmap shared memory." << std::endl;
    }
    shm_unlink(SHM_NAME);
}

bool Tasma::dodaj_cegle(int masa_cegly) {
    std::unique_lock<std::mutex> lock(mtx_);

    // Sprawdzanie, czy jest miejsce na taśmie i czy masa nie zostanie przekroczona
    if (tasma_[count_] < maks_liczba_cegiel_ && (tasma_[aktualna_masa_] + masa_cegly) <= maks_masa_) {
        tasma_[tasma_[rear_]] = masa_cegly;  // Dodanie cegły na taśmę
        tasma_[rear_] = (tasma_[rear_] + 1) % maks_liczba_cegiel_;
        tasma_[count_]++;
        tasma_[aktualna_masa_] += masa_cegly;

        std::string message = "Dodano cegłę o masie " + std::to_string(masa_cegly) + ". Aktualna masa: " + std::to_string(tasma_[aktualna_masa_]) + ". Ilość cegieł: " + std::to_string(tasma_[count_]);
        std::cout << "\x1b[32m" << message << "\x1b[0m" << std::endl;

        cv_.notify_one();  // Powiadomienie, że jest cegła do pobrania
        return true;
    } else {
        // Taśma jest pełna lub masa zostanie przekroczona
        return false;
    }
}

int Tasma::pobierz_cegle() {
    std::unique_lock<std::mutex> lock(mtx_);

    // Czekaj, aż na taśmie pojawi się cegła
    cv_.wait_for(lock, std::chrono::milliseconds(500), [this]{ return tasma_[count_] > 0; });

    // Pobranie cegły
    if (tasma_[count_] > 0) {
        int masa_cegly = tasma_[tasma_[front_]];
        tasma_[tasma_[front_]] = 0;  // Oczyszczamy miejsce na taśmie
        tasma_[front_] = (tasma_[front_] + 1) % maks_liczba_cegiel_;
        tasma_[count_]--;
        tasma_[aktualna_masa_] -= masa_cegly;

        std::string message1 = "Pobrano cegłę o masie " + std::to_string(masa_cegly) + ". Pozostała masa na taśmie: " + std::to_string(tasma_[aktualna_masa_]);
        std::cout << "\x1b[32m" << message1 << "\x1b[0m" << std::endl;

        return masa_cegly;
    }
    return 0;  // Brak cegieł
}

int Tasma::sprawdz_cegle() {
    std::unique_lock<std::mutex> lock(mtx_);

    cv_.wait_for(lock, std::chrono::milliseconds(500), [this]{ return tasma_[count_] > 0; });

    return tasma_[tasma_[front_]];  // Zwraca masę cegły na taśmie
}

bool Tasma::czy_pusta() const {
    return tasma_[count_] == 0;
}
