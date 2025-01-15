// Dyspozytor.cpp
#include "Dyspozytor.hpp"
#include <iostream>

Dyspozytor::Dyspozytor(Tasma& tasma)
    : tasma_(tasma) {
    // Tworzymy 3 pracowników, używając std::make_unique
    pracownicy_.push_back(std::make_unique<Pracownik>(1, 1, tasma_));
    pracownicy_.push_back(std::make_unique<Pracownik>(2, 2, tasma_));
    pracownicy_.push_back(std::make_unique<Pracownik>(3, 3, tasma_));

    ciezarowki_.push_back(std::make_unique<Ciezarowka>(15, 1, tasma_));
    ciezarowki_.push_back(std::make_unique<Ciezarowka>(15, 2, tasma_));
    ciezarowki_.push_back(std::make_unique<Ciezarowka>(15, 3, tasma_));


}

Dyspozytor::~Dyspozytor() {
    sygnal2();
}

void Dyspozytor::startPracownikow() {
    std::cout << "Dyspozytor rozpoczyna pracę pracowników.\n";

    for (auto& pracownik : pracownicy_) {
        pracownik->start();  // Uruchomienie pracy każdego pracownika
    }
}

void Dyspozytor::sygnal2() {
    std::cout << "Dyspozytor zatrzymuje pracowników.\n";
    
    for (auto& pracownik : pracownicy_) {
        pracownik->stop();  // Zatrzymanie każdego pracownika
        if (pracownik->getThread().joinable()) {
            pracownik->getThread().join();  // Czekamy na zakończenie wątku pracownika
        }
    }
}

void Dyspozytor::sygnal1(){
    std::cout<<"Dyspozytor wysyła ciężarówkę. \n";

}

void Dyspozytor::startCiezarowek(){
    std::cout << "Dyspozytor rozpoczyna pracę ciężarówek.\n";

    for (auto& ciezarowka : ciezarowki_) {
        ciezarowka->start();  // Uruchamiamy wątki ciężarówek
    }

    // Początkowe powiadomienie ciężarówek, aby zaczęły pracować naprzemiennie
    //ciezarowki_[0]->zacznij();  // Powiadomienie pierwszej ciężarówki
    //ciezarowki_[1]->zacznij();
}
