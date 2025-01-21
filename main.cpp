#include "include.hpp"
#include "BledneDane.hpp"
using namespace std;

int main() {

    pid_t pid = getpid();  // Pobieramy PID aktualnego procesu
    std::cout << "PID aktualnego procesu: " << pid << std::endl;

    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigusr1_handler);
    
    const std::string filename = "output_log.txt";

    if (!file_exists(filename)) {
        std::cerr << "Plik nie istnieje!.\n";
        perror("Błąd podczas otwierania pliku");
        return 1; 
    }

    // Próba otwarcia pliku w trybie do zapisu
    log_file.open(filename, std::ios::out | std::ios::app);
    if (!log_file.is_open()) {
        std::cerr << "Nie udało się otworzyć pliku do zapisu!" << std::endl;
        perror("Błąd przy otwieraniu pliku");
        return 1;
    }
    // Parametry taśmy
    int maks_liczba_cegiel = 10; // Maksymalna liczba cegieł
    int maks_masa = 15;          // Maksymalna masa na tasmie
    int ladownosc_ciezarowki = 15; // Ładowność każdej ciężarówki
    int liczba_ciezarowek = 3;    // Liczba dostępnych ciężarówek
    int czas_trwania_symulacji = 3;  //dokladniej to ilosc rund
    int liczba_signal1 = 2;  //ile chcemy zeby dyspozytor wydal syngnalow

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
        dyspozytor = &mydyspozytor; // wskaźnik na obiekt Dyspozytor
        mydyspozytor.startPracownikow();
        mydyspozytor.startCiezarowek();
        //tu sie wykonuje 
        for (int i = 0; i < liczba_signal1; i++)
            {
             std::this_thread::sleep_for(std::chrono::seconds(czas_trwania_symulacji));
             if(i!=liczba_signal1-1)  mydyspozytor.sygnal1();
            
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
