#include "Ciezarowka.hpp"
#include <chrono>
#include <iostream>
#include "Tasma.hpp"
#include "Dyspozytor.hpp"

std::mutex mtxi;
sem_t load_semaphore_;

Ciezarowka::Ciezarowka(int ladownosc, int id, Tasma &tasma, Dyspozytor &dyspozytor)
    : ladownosc_(ladownosc), id_(id), tasma_(tasma), dyspozytor_(dyspozytor), running_(false), ready_to_load_(false)
{
    // Inicjalizacja semafora dla synchronizacji wątków
    if (sem_init(&load_semaphore_, 0, 1) != 0)
    {
        perror("sem_init failed");
        exit(1);
    }
}

Ciezarowka::~Ciezarowka()
{
    stop();
    if (thread_.joinable())
    {
        thread_.join(); // Zatrzymuje wątek, jeżeli jest uruchomiony
    }
    if (sem_destroy(&load_semaphore_) != 0)
    {
        perror("sem_destroy failed"); // Usuwanie semafora
    }
}

void Ciezarowka::start()
{
    running_ = true;
    thread_ = std::thread(&Ciezarowka::load, this); // Uruchomienie wątku
    std::cout << "\033[35mCiężarówka " << id_ << " rozpoczęła prace.\n\033[0m";

}

void Ciezarowka::stop()
{
    // Zatrzymanie wątku
    running_ = false;
}

void Ciezarowka::load()
{
    // Pętla odpowiedzialna za ładowanie cegieł na ciężarówkę
    while (true)
    {
        int current_load = 0;
        sem_wait(&load_semaphore_); // Zatrzymanie dostępu dla innych wątków
        while (running_)
        {
            ready_to_load_ = true;
            int masa_cegly = tasma_.sprawdz_cegle(); // Sprawdzanie dostępnej cegły
            std::cout << "\033[35mCiężarówka " << id_ << " sprawdza teoretyczną cegłę o masie: " << std::to_string(masa_cegly) << "\033[0m" << std::endl;
            

            // Sprawdzenie, czy ładunek nie przekroczy ładowności
            if (current_load + masa_cegly > ladownosc_)
            {
                running_ = false; 
            }
            else if (masa_cegly != 0)
            {
                masa_cegly = tasma_.pobierz_cegle(); 
                current_load += masa_cegly;
                if (masa_cegly != 0)
                {
                    std::cout << "\033[35mCiężarówka " << id_ << " załadowała cegłę o masie " << masa_cegly << ". "
                     << "Aktualny ładunek: " << current_load << "/" << ladownosc_ << "\n\033[0m";
                }
            }
            

            std::this_thread::sleep_for(std::chrono::milliseconds(CZAS_ZALADUNKU)); // Symulacja czasu załadunku

            // Sprawdzenie, czy dyspozytor zatrzymał proces i taśma jest pusta
            if (dyspozytor_.getCzyZatrzymal() && tasma_.czy_pusta())
            {
                sem_post(&load_semaphore_); // Podnosi semafor, pozwalając innym wątkom kontynuować
                if (current_load != 0)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(CZAS_ROZLADUNKU)); // Czas na rozładunek
                    std::cout << "\033[35mOstatnia ciężarówka " << id_ << " dowiozła cegły.\033[0m" << std::endl;
                }
                else
                {
                    std::cout << "\033[35mCiężarówka " << id_ << " konczy zywot \033[0m" << std::endl;
                }
                stop();
                return;
            }
        }
        std::cout << "\033[35mCiężarówka " << id_ << " jest gotowa do odjazdu. " << "Aktualny ładunek: " << current_load << "\n\033[0m";
        ready_to_load_ = false;
        sem_post(&load_semaphore_); // Podnosi semafor po zakończeniu ładowania
        std::this_thread::sleep_for(std::chrono::milliseconds(CZAS_ROZLADUNKU)); // Czas na rozładunek
        
        
        
        running_ = true;
    }
}

int Ciezarowka::getID() const
{
    return id_; 
}

bool Ciezarowka::isReady()
{
    return ready_to_load_; 
}

bool Ciezarowka::sprawdzStan(Dyspozytor &dyspozytor)
{
    bool czy = dyspozytor.getCzyZatrzymal();
    return czy; 
}
