#include "Pracownik.hpp"
#include <chrono>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <csignal> // Do obsługi sygnałów (np. zatrzymanie procesu)

Pracownik::Pracownik(int id, int masa_cegly,Tasma& tasma)
    : id_(id), masa_cegly_(masa_cegly),czas_pracy_pracownika(500), tasma_(tasma), running_(false), pid_(-1) {}

Pracownik::~Pracownik() {
    stop();
}

void Pracownik::start() {
    running_ = true;
    
    pid_ = fork();  // Tworzymy nowy proces

    if (pid_ == -1) {
        std::cerr << "Błąd podczas tworzenia procesu." << std::endl;
    } else if (pid_ == 0) {
        // Kod wykonywany przez proces potomny
        // Zmiana nazwy procesu 
        std::string process_name = "Pracownik: " + std::to_string(id_);
        prctl(PR_SET_NAME, process_name.c_str(), 0, 0, 0);  // Ustawienie nazwy procesu
        produce(); // Uruchomienie funkcji produkcji, tutaj dzieje się cała praca pracownika
        exit(0); //proces potomny się kończy po wykonaniu zadania
    } else {
        // Kod wykonywany przez proces rodzica
        std::string message2 = "Pracownik P" + std::to_string(id_) + " rozpoczął pracę.\n";
        std::cout << "\x1b[94m" << message2 << "\x1b[0m";
    }
}

void Pracownik::stop() {
    if (running_ && pid_ > 0) {
        // Wysyłamy sygnał do zatrzymania procesu potomnego
        kill(pid_, SIGTERM);
        int status;
        waitpid(pid_, &status, 0); // Czekamy na zakończenie procesu
        std::string message = "Pracownik P" + std::to_string(id_) + " został zatrzymany.\n";
        std::cout << "\x1b[94m" << message << "\x1b[0m";
    }
    running_ = false;
}

int Pracownik::getID() const {
    return id_;
}

void Pracownik::produce() {
    while (running_) { // Pętla będzie działać dopóki 'running_' jest true, czyli przez cały czas trwania programu
        bool dodano = tasma_.dodaj_cegle(masa_cegly_);

        if (dodano) {
            std::string message3 = "Pracownik P" + std::to_string(id_) + " dodał cegłę o masie " + std::to_string(masa_cegly_) + ".\n";
            std::cout << "\x1b[94m" << message3 << "\x1b[0m";
        } else {
            std::string message = "Pracownik P" + std::to_string(id_) + " nie mógł dodać cegły - taśma pełna lub przekroczono masę.\n";
            std::cout << "\x1b[94m" << message << "\x1b[0m";
        }
      
        std::this_thread::sleep_for(std::chrono::milliseconds(czas_pracy_pracownika)); // czas potrzebny na polozenie cely na tasme
    }

    std::string message1 = "Pracownik P" + std::to_string(id_) + " zakończył pracę.\n";
    std::cout << "\x1b[94m" << message1 << "\x1b[0m";
}


