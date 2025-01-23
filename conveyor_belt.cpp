#include "conveyor_belt.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

// Inicjalizacja taśmy transportowej w pamięci współdzielonej
ConveyorBelt* initConveyorBelt() {
    // Tworzymy pamięć współdzieloną
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(ConveyorBelt));
    ConveyorBelt* belt = (ConveyorBelt*)mmap(0, sizeof(ConveyorBelt), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Inicjalizacja taśmy
    belt->front = 0;
    belt->rear = 0;
    belt->totalWeight.store(0);
    
    // Tworzymy semafor do synchronizacji
    belt->mutex = sem_open(SEM_MUTEX_NAME, O_CREAT, 0666, 1);

    return belt;
}

// Dodawanie cegły na taśmę
void addBrick(ConveyorBelt* belt, int weight) {
    sem_wait(belt->mutex);  // Czekamy na dostęp

    if ((belt->rear + 1) % MAX_BRICKS != belt->front && belt->totalWeight<MAX_BRICKS_W) {  // Sprawdzamy, czy jest miejsce w kolejce
        belt->bricks[belt->rear] = weight;
        belt->rear = (belt->rear + 1) % MAX_BRICKS;
        belt->totalWeight.fetch_add(weight, std::memory_order_relaxed);
        std::cout << "Cegła o wadze " << weight << " dodana. Całkowita masa: " << belt->totalWeight.load() << std::endl;
    } else {
        std::cout << "Brak miejsca na taśmie!" << std::endl;
    }

    sem_post(belt->mutex);  // Zwolnienie semafora
}

// Wyświetlanie stanu taśmy
void printBelt(ConveyorBelt* belt) {
    sem_wait(belt->mutex);  // Czekamy na dostęp

    std::cout << "Cegły na taśmie: ";
    int i = belt->front;
    while (i != belt->rear) {
        std::cout << belt->bricks[i] << " ";
        i = (i + 1) % MAX_BRICKS;
    }
    std::cout << std::endl;

    sem_post(belt->mutex);  // Zwolnienie semafora
}

// Czyszczenie zasobów
void cleanupConveyorBelt(ConveyorBelt* belt) {
    sem_close(belt->mutex);
    sem_unlink(SEM_MUTEX_NAME);
    munmap(belt, sizeof(ConveyorBelt));
    shm_unlink(SHM_NAME);
}
