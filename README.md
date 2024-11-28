# Temat 2 - Cegielnia

### Celem programu jest symulowanie pracy systemu, w którym:
- Pracownicy wrzucają cegły na taśmę,
- Taśma transportowa przesyła cegły do ciężarówek,
- Ciężarówki są załadowywane i odjeżdżają, a nowa ciężarówka przyjeżdża na miejsce,
- Dyspozytor może wydawać komendy do zakończenia pracy i odjazdu ciężarówek.
  
### Elementy do implementacji:
- symulacja taśmy transportowej: przechowywanie cegieł i sprawdzanie na bieżąco, czy ich liczba nie przekroczyła maxa K, a masa M,
- symulacja pracowników (P1, P2, P3): wrzucanie odpowiednich cegieł (o masach 1,2,3), aby nie zaszła sytuacja (3K>M),
- symulacja ciężarówki: odbieranie cegieł, transportowanie w czasie T, cegły mają trafić w tej samej kolejności, ładowność cieżarówki to C, a ich łączna liczba to N,
- symulacja dyspozytora: decydowanie o odjeździe ciężarówek - sygnał 1, zakonczenie pracy - sygnał 2
- sprawadzanie czy wszystkie dane wprowadzane przez użytkownika są poprawne.
- zaimplementowanie obsługi błędów używając funkcji bibliotecznej perror() i zmiennej errno.
- ustawienie minimalne prawa dostępu do tworzonych struktur.
- usunięcie wszystkich struktur po zakonczeniu zadania.
  
### Testy:
- Testowanie poprawności załadunku cegieł na taśmę.
- Sprawdzenie, czy taśma nie przekroczy dozwolonej liczby cegieł i masy.
- Testowanie działania ciężarówek – czy są one prawidłowo załadowane do pełna, a po załadowaniu wracają do cegielni.
- Testowanie interakcji z dyspozytorem – sprawdzenie, czy sygnały 1 (odjazd z niepełnym ładunkiem) i 2 (zakończenie pracy) działają poprawnie.
- symulacja wprowadzenia "złych" danych, na przyklad ustawienie ilosci ciezarowek lub M na 1.
  
 ### Podsumowanie:
 Zadanie związane jest z symulacją procesów oraz zarządzaniem zadaniami, które muszą być zsynchronizowane. Należy mieć na uwadze ograniczenia transportowe, kordynacje pracowników, cieżarówek oraz dyspozytorów.
  
### Link
https://github.com/Mciesielczyk/Cegielnia.git
