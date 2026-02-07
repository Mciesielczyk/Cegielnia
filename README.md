# Raport projektu – Cegielnia

---

## 1. Założenia projektowe

Projekt dotyczy symulacji działania cegielni, w której przy taśmie transportowej pracuje trzech pracowników oznaczonych jako **P1, P2 i P3**. Każdy z pracowników wrzuca na taśmę cegły o masie odpowiednio **1, 2 i 3 jednostki**.

System opiera się na następujących założeniach:

- Na taśmie może znajdować się maksymalnie **K cegieł**, przy czym ich łączna masa nie może przekraczać **M jednostek**.
- Na końcu taśmy stoi ciężarówka o ładowności **C jednostek**, którą należy załadować do pełna, zachowując kolejność cegieł z taśmy.
- Po zapełnieniu ciężarówki pojawia się nowa (jeżeli jest dostępna). Łączna liczba ciężarówek wynosi **N**.
- Każda ciężarówka po załadunku rozwozi cegły i wraca po czasie **Ti**.
- Dyspozytor może wysłać:
  - **sygnał 1** – wymuszenie odjazdu ciężarówki z niepełnym ładunkiem,
  - **sygnał 2** – zakończenie pracy pracowników i ciężarówek po zakończeniu produkcji.

---

## 2. Opis kodu

### Klasa `Ciezarowka`
- Reprezentuje ciężarówki odbierające cegły z taśmy.
- Każda ciężarówka ma określoną ładowność.
- Standardowo musi zostać załadowana do pełna, chyba że dyspozytor wymusi wcześniejszy wyjazd.
- Po załadunku opuszcza cegielnię, rozwozi cegły i wraca po określonym czasie.
- Synchronizacja zarządza załadunkiem i odjazdem.

### Klasa `Dyspozytor`
- Główny element sterujący systemem.
- Obsługuje sygnały:
  - **Sygnał 1** – wymuszenie odjazdu niepełnej ciężarówki,
  - **Sygnał 2** – zakończenie pracy systemu.
- Monitoruje stan taśmy oraz dostępność ciężarówek.

### Klasa `Pracownik`
- Reprezentuje pracowników **P1, P2, P3**.
- Dodają cegły na taśmę transportową.
- Synchronizacja dostępu do taśmy odbywa się przy użyciu semaforów.

### Klasa `Tasma`
- Zarządza taśmą transportową.
- Ograniczenia:
  - maksymalna liczba cegieł (**K**),
  - maksymalna masa (**M**).
- Wykorzystuje kolejkę FIFO do zachowania kolejności cegieł.
- Mechanizmy synchronizacji zapewniają:
  - brak przepełnienia taśmy,
  - poprawny odbiór cegieł przez ciężarówki.

---

## 3. Osiągnięcia projektu

- Implementacja systemu z użyciem procesów i wątków.
- Synchronizacja za pomocą semaforów.
- Kontrola pojemności i masy taśmy transportowej.
- Obsługa sygnałów systemowych.
- Logowanie działań do pliku `output_log.txt`.
- Kolorowe komunikaty w konsoli.
- Dynamiczne ustawianie parametrów symulacji.

---

## 4. Problemy napotkane podczas realizacji

- Ryzyko zakleszczeń przy synchronizacji semaforów.
- Konflikty przy wypisywaniu na `cout` z wielu wątków.
- Sprawdzanie czy ciężarówka jest w pełni załadowana.
- Problemy z pamięcią dzieloną i umieszczaniem kolejki w niej.
- Zawieszanie się `sem_wait()`.
- Pobieranie ujemnej liczby cegieł przy długiej symulacji.

---

## 5. Dodane elementy specjalne

- Kolorowe komunikaty w konsoli.
- Obsługa sygnałów:
  - `SIGINT` – zakończenie pracy dyspozytora,
  - `SIGQUIT` – przekierowanie danych do logów,
  - `SIGUSR1` – zakończenie programu.
- Dynamiczne wprowadzanie parametrów:
  - `std::cin.fail()`
  - `std::cin.clear()`
  - `std::cin.ignore()`
- Logowanie danych do `output_log.txt` przy użyciu `std::ofstream`.

---

## 6. Testy

- Masa = 0 → pracownicy nie mogą dodawać cegieł.
- Masa = 1 → tylko pracownik P1 może dodawać cegły.
- Ładowność ciężarówki = 3 → działa wolno, ale poprawnie.
- Liczba ciężarówek = 0 → taśma się przepełnia.
- Czas symulacji = 0 → program kończy się natychmiast.
- Test długiej symulacji (300s) – bez błędów.
- Sprawdzanie poprawnego usuwania semaforów i pamięci dzielonej.

---

## 7.Użyte mechanizmy

### Pliki
log_file.open() – otwieranie pliku w trybie zapisu. 

file_exists() – sprawdzanie istnienia pliku. 

### Pamięć dzielona
- `shm_open()`
- `ftruncate()`
- `mmap()`
- `munmap()`
- `shm_unlink()`

### Synchronizacja
- `sem_open()`
- `sem_wait()`
- `sem_post()`
- `sem_close()`
- `sem_unlink()`

### Sygnały
- `signal()`
- `SIGINT`, `SIGQUIT`, `SIGUSR1`

### Wyjątki
- `try`, `catch`, `throw`
- `perror()`

### Procesy
- `fork()`
- `prctl()`
- `exit()`
- `waitpid()`
- `kill()`

### Wątki
- `std::thread`
- `sleep_for()`
- `joinable()`
- `join()`

---

## Kompilacja

```bash
g++ -std=c++14 main.cpp Pracownik.cpp Tasma.cpp Dyspozytor.cpp Ciezarowka.cpp Mutex.cpp include.cpp -o cegielnia -pthread -lrt
