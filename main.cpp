#include "include.hpp"
#include "BledneDane.hpp"
using namespace std;

int main() {

    pid_t pid = getpid();  // Pobieramy PID aktualnego procesu
    std::cout << "PID aktualnego procesu: " << pid << std::endl;
    // Rejestracja funkcji obsługi sygnałów:
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigusr1_handler);
    signal(SIGUSR1, handleSignal);

    const std::string filename = "output_log.txt";

    // Sprawdzenie, czy plik istnieje przed próbą jego otwarcia
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
    int maks_masa = 20;          // Maksymalna masa na tasmie
    int ladownosc_ciezarowki = 12; // Ładowność każdej ciężarówki
    int liczba_ciezarowek = 3;    // Liczba dostępnych ciężarówek
    int czas_trwania_symulacji = 4;  //czas w jednej rundzie
    int liczba_signal1 = 2;  //ile chcemy zeby dyspozytor wydal syngnalow/ ilosc rund


    try {
         std::cout << "WITAJ UZYTKOWNIKU" << std::endl << "WPROWADZ: " << std::endl;
         maks_liczba_cegiel = wczytajInt("- MAKYMALNA LICZBE CEGIEL NA TASMIE: ");
         maks_masa = wczytajInt("- MAKSYMALNA MASE CEGIEL NA TASMIE: ");
         ladownosc_ciezarowki = wczytajLadownosc();
         liczba_ciezarowek = wczytajInt("- LICZBA CIEŻARÓWEK: ");
         liczba_signal1 = wczytajInt("- LICZBA SYGNAŁÓW 1: ");
         czas_trwania_symulacji = wczytajInt("- CZAS TRWANIA SYMULACJI (w sekundach): ");

        czas_trwania_symulacji = czas_trwania_symulacji / liczba_signal1;

        Tasma tasma(maks_liczba_cegiel, maks_masa);
        Dyspozytor mydyspozytor(tasma,liczba_ciezarowek,ladownosc_ciezarowki);
        dyspozytor = &mydyspozytor; // wskaźnik na obiekt Dyspozytor
        mydyspozytor.startPracownikow();
        mydyspozytor.startCiezarowek();
        //tu sie wykonuje 
        for (int i = 0; i < liczba_signal1; i++)
            {
            std::unique_lock<std::mutex> lock(mtx);  // Lock na mutexie
           // Czekamy na zdarzenie przez określony czas, ale jeśli flaga1 jest ustawiona na true, przerwiemy oczekiwanie
            cv.wait_for(
                 lock,  // Mutex, który jest trzymany przez wątek
                 std::chrono::seconds(czas_trwania_symulacji),  // Określony czas oczekiwania
                 [] {  // Funkcja lambda, która zwraca true, jeśli flaga1 jest ustawiona na true
                 return flaga1.load(); 
                 }
                );


            if(i!=liczba_signal1-1)  mydyspozytor.sygnal1();
            
            //mozliwosc korzystania z ctrl+c do wysylania sygnalu
           if (flaga1 == true) {
                std::cout << "Pętla przerwana.\n";
                break;  // Wyjście z pętli po otrzymaniu sygnału
            }
            }
    

    // Zatrzymanie pracy
   
    mydyspozytor.sygnal2();
    
      

    } catch (const std::exception& e) { // Obsługa wyjątków, jeżeli wystąpił jakikolwiek błąd
        std::cerr << "Nieoczekiwany błąd: " << e.what() << std::endl;
        return 1; 
    }
    
    return 0;
}
