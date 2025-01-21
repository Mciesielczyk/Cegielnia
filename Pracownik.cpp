#include "Pracownik.hpp"
#include <chrono>
#include <iostream>

Pracownik::Pracownik(int id, int masa_cegly, Tasma& tasma)
    : id_(id), masa_cegly_(masa_cegly), tasma_(tasma), running_(false) {}

Pracownik::~Pracownik() {
    stop();
    if (thread_.joinable()) {
        thread_.join();
    }
}

void Pracownik::start() {
    running_ = true;
    thread_ = std::thread(&Pracownik::produce, this);
    std::string message2 = "Pracownik P" + std::to_string(id_) + " rozpoczął pracę.\n";
    std::cout << "\x1b[94m" << message2<< "\x1b[0m";
}

void Pracownik::stop() {
    running_ = false;
}

int Pracownik::getID() const {
    return id_;
}

void Pracownik::produce() {
    while (running_) {
        
        bool dodano = tasma_.dodaj_cegle(masa_cegly_);

        if (dodano) {
            std::string message3= "Pracownik p"+std::to_string(id_)+" dodal cegle o masie " + std::to_string(masa_cegly_)+".\n";
            std::cout<<"\x1b[94m"<<message3<<"\x1b[0m";
        } else {
            std::string message = "Pracownik P" + std::to_string(id_) + " nie mógł dodać cegły - taśma pełna lub przekroczono masę.\n";
            std::cout << "\x1b[94m" << message<< "\x1b[0m";
        }

        // Symulacja czasu dodawania cegły
        std::this_thread::sleep_for(std::chrono::milliseconds(600)); // Dostosuj czas w razie potrzeby
    }
    std::string message1 = "Pracownik P" + std::to_string(id_) + " zakończył pracę.\n";
    std::cout << "\x1b[94m" << message1<< "\x1b[0m";

}
