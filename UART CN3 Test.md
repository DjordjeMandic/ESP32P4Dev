# Testiranje UART komunikacije (CN3 konektor, ESP32-P4)

## Cilj testiranja

Cilj ovog testa je verifikacija funkcionalnosti **UART interfejsa dostupnog na CN3 konektoru** na ESP32-P4 platformi.

---

## Metodologija

Za testiranje je korišćen `uart_echo` primer iz ESP-IDF okruženja.

- Firmware je moguće flashovati:
    - putem integrisanog USB-Serial interfejsa
    - ili eksternog USB-Serial adaptera
- Za samu UART komunikaciju tokom testa korišćen je:
    - **eksterni USB-Serial adapter**

---

## Konfiguracija pinova

Za pravilno mapiranje UART signala na CN3 konektor potrebno je podesiti sledeće:

```cpp
CONFIG_EXAMPLE_UART_RXD=4
CONFIG_EXAMPLE_UART_TXD=5
```

- RXD → GPIO4
- TXD → GPIO5

---

## Test konfiguracija (direktna komunikacija)

Eksterni USB-Serial adapter povezan je na sledeći način:

- RX (adapter) → TX (STDIO, GPIO37 na ESP32-P4)
- TX (adapter) → RX (CN3, GPIO4)

Ova konfiguracija omogućava da se podaci poslati iz terminala proslede aplikaciji, a zatim odmah vrate i ispišu putem log sistema.

### Rezultat

```id="6b9ydg"
I (277) main_task: Calling app_main()
I (277) main_task: Returned from app_main()
I (69357) UART TEST: Recv str: T
I (69537) UART TEST: Recv str: E
I (69767) UART TEST: Recv str: S
I (69927) UART TEST: Recv str: T
```

**Zaključak:**  
Aplikacija uspešno prima karaktere putem CN3 UART interfejsa i obrađuje ih.

---

## Loopback test (CN3)

Za dodatnu verifikaciju izvršen je **loopback test** direktnom vezom eksternog UART interfejsa na CN3

U ovom režimu, svaki poslati karakter se direktno vraća nazad na prijem.

### Rezultat

```id="r9s4pl"
I (277) main_task: Calling app_main()
I (277) main_task: Returned from app_main()
I (69357) UART TEST: Recv str: T
I (69537) UART TEST: Recv str: E
I (69767) UART TEST: Recv str: S
I (69927) UART TEST: Recv str: T
/* sadrzaj iznad je od ranije, novi sadrzaj je ispod ovog komentara */
TEST
```

- Terminal prikazuje kompletan string (`TEST`) zahvaljujući loopback-u

---

## Zaključak

Testiranjem je potvrđeno sledeće:

- UART interfejs na CN3 konektoru je funkcionalan
- Prijem podataka putem RX linije radi ispravno
- TX linija funkcioniše, što je potvrđeno loopback testom
- `uart_echo` primer uspešno demonstrira dvosmernu komunikaciju
