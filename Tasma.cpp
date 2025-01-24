#include "Tasma.hpp"
#include <iostream>
#include <stdexcept>
#include <semaphore.h>  // Nagłówek dla semaforów POSIX

Tasma::Tasma(int maks_liczba_cegiel, int maks_masa)
    : maks_liczba_cegiel_(maks_liczba_cegiel), maks_masa_(maks_masa), shm_fd_(-1), shm_size_(sizeof(SharedQueue)) {
    utworzPamiecDzielona();
    // Tworzenie semaforów
    sem_mutex_ = sem_open("/semafor_mutex", O_CREAT, 0666, 1);          // Semafor do wzajemnego wykluczania
    sem_space_available_ = sem_open("/semafor_space", O_CREAT, 0666, 1000); // Semafor na miejsce w kolejce
    sem_items_available_ = sem_open("/semafor_items", O_CREAT, 0666, 0);    // Semafor do dostępnych cegieł
}

Tasma::~Tasma() {
    zwolnijPamiecDzielona();
    // Usuwanie semaforów
    sem_close(sem_mutex_);
    sem_unlink("/semafor_mutex");

    sem_close(sem_space_available_);
    sem_unlink("/semafor_space");

    sem_close(sem_items_available_);
    sem_unlink("/semafor_items");
}

void Tasma::utworzPamiecDzielona() {
    // Tworzenie/otwieranie pamięci dzielonej
    shm_fd_ = shm_open(shm_name_, O_CREAT | O_RDWR, 0666);
    if (shm_fd_ == -1) {
        perror("shm_open");
        throw std::runtime_error("Nie udało się utworzyć pamięci dzielonej");
    }

    // Ustawienie rozmiaru pamięci dzielonej
    if (ftruncate(shm_fd_, shm_size_) == -1) {
        perror("ftruncate");
        close(shm_fd_);
        throw std::runtime_error("Nie udało się ustawić rozmiaru pamięci dzielonej");
    }

    // Mapowanie pamięci dzielonej do przestrzeni adresowej procesu
    void* ptr = mmap(nullptr, shm_size_, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        close(shm_fd_);
        throw std::runtime_error("Nie udało się zamapować pamięci dzielonej");
    }

    shared_queue_ = static_cast<SharedQueue*>(ptr);

    // Inicjalizacja kolejki w pamięci dzielonej
    shared_queue_->head = 0;
    shared_queue_->tail = 0;
    shared_queue_->aktualna_masa_ = 0;

    memset(shared_queue_->data, 0, sizeof(shared_queue_->data));
}

void Tasma::zwolnijPamiecDzielona() {
    if (shared_queue_) {
        munmap(shared_queue_, shm_size_);
        shared_queue_ = nullptr;
    }
    if (shm_fd_ != -1) {
        close(shm_fd_);
        shm_unlink(shm_name_);
        shm_fd_ = -1;
    }
}

bool Tasma::dodaj_cegle(int masa_cegly) {
    // Zablokowanie semafora mutex
    sem_wait(sem_mutex_);

    // Sprawdzenie, czy można dodać cegłę
    bool jest_miejsce_na_cegle = (shared_queue_->tail + 1) % 1000 != shared_queue_->head;
    bool masa_w_normie = (shared_queue_->aktualna_masa_ + masa_cegly <= maks_masa_);

    if (jest_miejsce_na_cegle && masa_w_normie) {
        // Dodawanie cegły
        shared_queue_->data[shared_queue_->tail] = masa_cegly;
        shared_queue_->tail = (shared_queue_->tail + 1) % 1000;
        shared_queue_->aktualna_masa_ += masa_cegly;

        std::cout << "Dodano cegłę o masie: " << masa_cegly << "\n";
        
        // Odblokowanie semafora mutex
        sem_post(sem_mutex_);
        
        // Zwiększenie semafora items_available, bo cegła jest dostępna
        sem_post(sem_items_available_);

        return true;
    }

    // Odblokowanie semafora mutex, jeśli nie dodano cegły
    sem_post(sem_mutex_);
    return false;
}

int Tasma::pobierz_cegle() {
    // Czekanie na dostępność cegły
    sem_wait(sem_items_available_);

    // Zablokowanie semafora mutex
    sem_wait(sem_mutex_);

    // Pobieranie cegły
    int masa_cegly = shared_queue_->data[shared_queue_->head];
    shared_queue_->head = (shared_queue_->head + 1) % 1000;
    shared_queue_->aktualna_masa_ -= masa_cegly;

    std::cout << "Pobrano cegłę o masie: " << masa_cegly << ". Aktualna masa: " << shared_queue_->aktualna_masa_ << "\n";
    
    // Odblokowanie semafora mutex
    sem_post(sem_mutex_);

    // Zwiększenie semafora space_available, bo jest miejsce na cegłę
    sem_post(sem_space_available_);

    return masa_cegly;
}

int Tasma::sprawdz_cegle() {
    // Zablokowanie semafora mutex
    sem_wait(sem_mutex_);

    int masa_cegly = shared_queue_->data[shared_queue_->head];

    // Odblokowanie semafora mutex
    sem_post(sem_mutex_);
    
    return masa_cegly;
}

bool Tasma::czy_pusta() const {
    return shared_queue_->head == shared_queue_->tail;
}

void Tasma::debugKolejka() {
    std::cout << "Head: " << shared_queue_->head
              << ", Tail: " << shared_queue_->tail
              << ", Aktualna masa: " << shared_queue_->aktualna_masa_ << "\n";
}
