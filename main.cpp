#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <random>
#include "Pracownik.hpp"
#include "Tasma.hpp"
#include "Ciezarowka.hpp"
#include "Dyspozytor.hpp"
#include "BledneDane.hpp"
#include <csignal>
#include <sstream>
#include <fstream>
#include <unistd.h>  // Dla getpid()

Dyspozytor* dyspozytor = nullptr;  
std::ofstream log_file("output_log.txt"); 
std::streambuf* old_buf = std::cout.rdbuf(); // Przechowujemy stary bufor


void sigint_handler(int signum) {
     if (dyspozytor != nullptr) {
        dyspozytor->sygnal1(); 
    }
}
void sigusr1_handler(int signum) {
    
    std::cout << "Otrzymano sygnał SIGQUIT. Zapisuję dane do pliku...\n"; //ctrl+ ukosnik
    std::cout.rdbuf(log_file.rdbuf());  // Przekierowujemy std::cout do pliku
}




using namespace std;
int main() {

    pid_t pid = getpid();  // Pobieramy PID aktualnego procesu
    std::cout << "PID aktualnego procesu: " << pid << std::endl;
    signal(SIGINT, sigint_handler);
    //signal(SIGTERM, sigterm_handler);
    signal(SIGQUIT, sigusr1_handler);
//kill -USR1 1730367

    // Parametry taśmy
    int maks_liczba_cegiel = 10; // Maksymalna liczba cegieł
    int maks_masa = 15;          // Maksymalna masa
    int ladownosc_ciezarowki = 15; // Ładowność każdej ciężarówki
    int liczba_ciezarowek = 3;    // Liczba dostępnych ciężarówek
    int czas_trwania_symulacji = 3; //podzielony przez 5
    int liczba_signal1 = 2;

    try {
        // std::cout << "WITAJ UZYTKOWNIKU" << std::endl << "WPROWADZ: " << std::endl;

        // maks_liczba_cegiel = wczytajInt("- MAKYMALNA LICZBE CEGIEL NA TASMIE: ");
        // maks_masa = wczytajInt("- MAKSYMALNA MASE CEGIEL NA TASMIE: ");
        // ladownosc_ciezarowki = wczytajInt("- ŁADOWNOŚĆ CIEŻARÓWKI: ");
        // liczba_ciezarowek = wczytajInt("- LICZBA CIEŻARÓWEK: ");
        // liczba_signal1 = wczytajInt("- LICZBA SYGNAŁÓW 1: ");
        // czas_trwania_symulacji = wczytajInt("- CZAS TRWANIA SYMULACJI (w sekundach): ");

        

       // czas_trwania_symulacji = czas_trwania_symulacji / liczba_signal1;
        Tasma tasma(maks_liczba_cegiel, maks_masa);
        Dyspozytor mydyspozytor(tasma,liczba_ciezarowek,ladownosc_ciezarowki);
        dyspozytor = &mydyspozytor; // Przypisujemy wskaźnik na obiekt Dyspozytor
        mydyspozytor.startPracownikow();
        mydyspozytor.startCiezarowek();
        for (int i = 0; i < liczba_signal1; i++)
            {
             std::this_thread::sleep_for(std::chrono::seconds(czas_trwania_symulacji));
             mydyspozytor.sygnal1();
            //mozliwosc korzystania z ctrl+c do wysylania sygnalu
            }
    

    // Zatrzymanie pracy
    mydyspozytor.sygnal2();
      

    } catch (const std::exception& e) {
        std::cerr << "Nieoczekiwany błąd: " << e.what() << std::endl;
        return 1; // Zakończenie programu z kodem błędu
    }
    
    return 0;
}
