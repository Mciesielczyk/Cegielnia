#include "include.hpp"
 

 
std::streambuf* old_buf = std::cout.rdbuf();

bool file_exists(const std::string& filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

void sigint_handler(int signum) {
     if (dyspozytor != nullptr) {
        dyspozytor->sygnal1(); 
    }
}

void handleSignal(int signal) {
    if (signal == SIGUSR1) {
        std::cout << "Otrzymano sygnał SIGUSR1!" << std::endl;
        dyspozytor->sygnal2();
    }
}

void sigusr1_handler(int signum) {
    
    std::cout << "Otrzymano sygnał SIGUSR1. Zapisuję dane do pliku...\n"; //ctrl+ ukosnik
    std::cout.rdbuf(log_file.rdbuf());  // Przekierowujemy std::cout do pliku
}

std::ofstream log_file;         // Definicja zmiennej
Dyspozytor* dyspozytor = nullptr; // Definicja zmiennej
