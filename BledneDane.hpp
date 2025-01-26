#pragma once
#include <exception>
#include <string>
#include <iostream>
#include <stdexcept>
#include <limits>


// Funkcja wczytująca liczbę całkowitą od użytkownika.
// Zwraca dodatnią liczbę całkowitą wprowadzaną przez użytkownika, 
// konwertując ewentualne liczby ujemne na dodatnie, 
// oraz wymusza, by wartość nie była zerem.

int wczytajInt(const std::string& prompt) {
    int liczba;
    while (true) {
        std::cout << prompt;
        std::cin >> liczba;

        // Sprawdzanie, czy wprowadzono poprawną liczbę całkowitą
        if (std::cin.fail()) {
            std::cin.clear(); // Usuwamy błąd strumienia
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Usuwamy pozostałości w strumieniu
            std::cout << "Błąd! Wprowadzono niepoprawną wartość. Spróbuj ponownie." << std::endl;
        } else {
            // Jeśli liczba jest ujemna, zamieniamy ją na dodatnią
            if (liczba < 0) {
                std::cout << "Liczba była ujemna, zamieniamy ją na dodatnią." << std::endl;
                liczba *= -1;  
                return liczba;
            }
            if (liczba == 0) {
                std::cout << "Błąd! Liczba nie może być zerem. Spróbuj ponownie." << std::endl;
            } else {
                return liczba;
            }
        }
    }
    
}
