# Testiranje USB-Serial komunikacije (ESP32-P4)

## Cilj testiranja

Cilj ovog testa je provera funkcionalnosti integrisanog **USB-Serial interfejsa** na ESP32-P4 platformi u aplikativnom režimu rada.

---

## Opis problema

Inicijalno je uočeno da **integrisani USB-Serial interfejs ne funkcioniše u aplikaciji**, već je dostupan isključivo u **boot režimu** (tokom flashovanja firmware-a).

Radi verifikacije, isti primer je testiran na drugoj razvojnoj ploči baziranoj na ESP32-C6, gde USB-Serial funkcioniše ispravno i u boot i u aplikativnom režimu. Time je isključena fabrička softverska greška u primeru i ukazano na potencijalni hardverski ili konfiguracioni problem specifičan za ESP32-P4 platformu.

---

## Analiza i uzrok problema

Daljom analizom utvrđeno je da uzrok problema verovatno nije u samom mikrokontroleru ili firmware-u.

Eksperimentalno je potvrđeno da:

> **Povezivanjem D+ i D− linija između headera H1 i H3 dolazi do ispravnog rada USB-Serial interfejsa i u aplikativnom režimu.**

---

## Napomena o reviziji čipa

Korišćeni mikrokontroler:
- **ESP32-P4, revizija v1.3**

Za ispravan rad neophodno je u svakom projektu eksplicitno podesiti odgovarajuću reviziju čipa.

---

## Alternativno rešenje (eksterni USB-Serial adapter)

Pre identifikacije uzroka, kao workaround korišćen je eksterni USB-Serial adapter:
- **CP2102 USB-Serial konverter**

### Povezivanje
- TX (adapter) → GPIO38 (RX na ESP32-P4)
- RX (adapter) → GPIO37 (TX na ESP32-P4)

Ova konfiguracija omogućava standardnu UART komunikaciju nezavisno od USB interfejsa.

---

## Test funkcionalnosti

Za testiranje je korišćen jednostavan “Hello World” program sa periodičnim restartom.

### Rezultat ispisa

```id="usg2fd"
Hello world!
This is esp32p4 chip with 2 CPU core(s), , silicon revision v1.3, 2MB external flash
Minimum free heap size: 614316 bytes
Restarting in 10 seconds...
Restarting in 9 seconds...
Restarting in 8 seconds...
Restarting in 7 seconds...
Restarting in 6 seconds...
Restarting in 5 seconds...
Restarting in 4 seconds...
Restarting in 3 seconds...
Restarting in 2 seconds...
Restarting in 1 seconds...
Restarting in 0 seconds...
```

---

## Zaključak

- Integrisani USB-Serial interfejs:
    - Funkcionalan u boot režimu
- Eksperimentalno rešenje:
    - Rešava se povezivanjem D+ i D− linija između headera **H1 i H3**
- Eksterni USB-Serial (CP2102):
    - Validan workaround kao i prethodni primer
- Obavezna konfiguracija:
    - Ispravno podešavanje revizije čipa (**v1.3**)
