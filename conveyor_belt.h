#ifndef CONVEYOR_BELT_H
#define CONVEYOR_BELT_H

#include <atomic>
#include <iostream>
#include <semaphore.h>

#define MAX_BRICKS 10  // Maksymalna liczba cegieł na taśmie
#define MAX_BRICKS_W 15 //max masa
#define SHM_NAME "/sharedConveyorBelt"  // Nazwa pamięci współdzielonej
#define SEM_MUTEX_NAME "/sem_mutex"  // Nazwa semafora

// Struktura do reprezentowania taśmy transportowej w pamięci współdzielonej
struct ConveyorBelt {
    int bricks[MAX_BRICKS];          // Tablica na cegły
    int front;                       // Indeks pierwszego elementu
    int rear;                        // Indeks ostatniego elementu
    std::atomic<int> totalWeight;    // Całkowita masa cegieł
    sem_t* mutex;                   // Semafor do synchronizacji
};

// Funkcja do inicjalizacji taśmy (pamięć współdzielona)
ConveyorBelt* initConveyorBelt();

// Funkcja do dodawania cegły na taśmę
void addBrick(ConveyorBelt* belt, int weight);

// Funkcja do wyświetlania stanu taśmy
void printBelt(ConveyorBelt* belt);

// Funkcja do usuwania zasobów
void cleanupConveyorBelt(ConveyorBelt* belt);

#endif // CONVEYOR_BELT_H
