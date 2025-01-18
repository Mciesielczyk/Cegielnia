// Dyspozytor.cpp
#include "Dyspozytor.hpp"
#include "Ciezarowka.hpp"
#include <iostream>

Dyspozytor::Dyspozytor(Tasma& tasma, int liczba_ciezarowek, int ladownosc_ciezarowki)
    : tasma_(tasma) {
    // Tworzymy 3 pracowników, używając std::make_unique
    pracownicy_.push_back(std::make_unique<Pracownik>(1, 1, tasma_));
    pracownicy_.push_back(std::make_unique<Pracownik>(2, 2, tasma_));
    pracownicy_.push_back(std::make_unique<Pracownik>(3, 3, tasma_));

    for (int i = 0; i < liczba_ciezarowek; ++i) {
        ciezarowki_.push_back(std::make_unique<Ciezarowka>(ladownosc_ciezarowki, i + 1, tasma_, *this));
    }

}

Dyspozytor::~Dyspozytor() {}

void Dyspozytor::startPracownikow() {
std::cout << "\x1b[35mDyspozytor rozpoczyna pracę pracowników.\x1b[0m" << std::endl;

    for (auto& pracownik : pracownicy_) {
        pracownik->start();  // Uruchomienie pracy każdego pracownika
    }
}

void Dyspozytor::sygnal2() {
std::cout << "\x1b[35mDyspozytor zatrzymuje pracowników. \x1b[0m" << std::endl;
    
    for (auto& pracownik : pracownicy_) {
        pracownik->stop();  // Zatrzymanie każdego pracownika
        if (pracownik->getThread().joinable()) {
            pracownik->getThread().join();  // Czekamy na zakończenie wątku pracownika
        }
    }
     czy_zatrzymal=true;

}

void Dyspozytor::sygnal1(){
    for(auto& ciezarowka : ciezarowki_){
        if(ciezarowka->isReady()){
                 std::cout << "\x1b[35mDyspozytor wysyła sygnal.------------------------ \x1b[0m" << std::endl;
                 ciezarowka->stop();
                 break;
        }
    }
}

void Dyspozytor::startCiezarowek(){
std::cout << "\x1b[35mDyspozytor rozpoczyna pracę ciężarówek. \x1b[0m" << std::endl;

    for (auto& ciezarowka : ciezarowki_) {
        ciezarowka->start();  // Uruchamiamy wątki ciężarówek
    }
}


bool Dyspozytor::getCzyZatrzymal(){
    return czy_zatrzymal;
}