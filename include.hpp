#pragma once
#include <iostream>    // Standardowa biblioteka do operacji wejścia/wyjścia, np. std::cout, std::cin
#include <thread>      // Biblioteka do obsługi wątków. Umożliwia tworzenie, synchronizowanie i zarządzanie wątkami.
#include <chrono>      // Biblioteka do manipulacji czasem i datą. Pomaga w opóźnieniach, mierzeniu czasu, itp.
#include <algorithm>   // Standardowa biblioteka zawierająca funkcje algorytmiczne, takie jak std::sort, std::find, itp.
#include <random>      // Biblioteka do generowania liczb losowych. Pozwala na tworzenie generatorów liczb losowych i rozkładów. Nie uzywana
#include "Pracownik.hpp" // Nagłówek zawierający deklaracje klasy Pracownik 
#include "Tasma.hpp"     // Nagłówek zawierający deklaracje klasy Tasma
#include "Ciezarowka.hpp" // Nagłówek zawierający deklaracje klasy Ciezarowka
#include "Dyspozytor.hpp" // Nagłówek zawierający deklaracje klasy Dyspozytor
#include <csignal>    // Biblioteka do obsługi sygnałów systemowych, takich jak SIGINT, SIGQUIT. Używana do odbierania sygnałów z systemu.
#include <sstream>    // Biblioteka do manipulacji strumieniami tekstowymi. Umożliwia konwersję między tekstem a innymi typami danych.
#include <fstream>    // Biblioteka do pracy z plikami (np. otwieranie, odczytywanie, zapisywanie do plików).
#include <unistd.h>   // Biblioteka systemowa w systemach Unix/Linux, zawierająca funkcje takie jak getpid() (pobieranie PID procesu).
#include <cstdio>     // Standardowa biblioteka C do obsługi funkcji wejścia/wyjścia, takich jak printf(), perror(), itp.
#include <cerrno>     // Zawiera definicje kodów błędów systemowych, np. errno, który przechowuje kody błędów funkcji systemowych.
#include <sys/stat.h> // Biblioteka systemowa z funkcjami do pracy z plikami i katalogami w systemach Unix/Linux. Zawiera np. stat() do uzyskania informacji o pliku.


// Sprawdza, czy plik o podanej nazwie istnieje
bool file_exists(const std::string& filename);

// Obsługuje sygnał SIGINT (CTRL+C)
void sigint_handler(int signum);

// Obsługuje sygnał SIGUSR1 (SIGQUIT CTRL+\ )
void sigusr1_handler(int signum);

// Obsługuje sygnał SIGUSR1 (kill -USR1 <PID>)
void handleSignal(int signum);

// plik logów, do którego będą zapisywane informacje
extern std::ofstream log_file;

// wskaźnik do obiektu Dyspozytor
extern Dyspozytor* dyspozytor;
