# Testiranje USB-Serial komunikacije (ESP32-P4)

## Cilj testiranja

Cilj ovog testa je provera funkcionalnosti integrisanog **USB-Serial/JTAG interfejsa** na ESP32-P4 platformi u aplikativnom režimu rada.

---

## Opis problema

Inicijalno je uočeno da integrisani **USB-Serial/JTAG interfejs** ne funkcioniše u aplikaciji, već je dostupan isključivo u **boot režimu** (tokom flashovanja firmware-a).

Radi verifikacije, isti primer je testiran na drugoj razvojnoj ploči baziranoj na ESP32-C6 mikrokontroleru, gde USB-Serial funkcioniše ispravno i u boot i u aplikativnom režimu. Time je isključena softverska greška u primeru i ukazano na potencijalni hardverski ili konfiguracioni problem specifičan za ESP32-P4 platformu.

---

## Analiza i uzrok problema

Daljom analizom šeme utvrđeno je da uzrok problema nije u samom mikrokontroleru niti firmware-u, već u povezivanju USB PHY interfejsa.

ESP32-P4 poseduje dva Full-Speed USB PHY interfejsa:

- **FS_PHY1** → GPIO24 / GPIO25
- **FS_PHY2** → GPIO26 / GPIO27

Podrazumevana konfiguracija mikrokontrolera je:

- **FS_PHY1** povezan na **USB Serial/JTAG kontroler**
- **FS_PHY2** povezan na **OTG_FS**

Mapiranje je moguće promeniti pomoću **eFuse konfiguracije**:

    - FS_PHY1 → USB Serial/JTAG
    - FS_PHY2 → OTG_FS

    - FS_PHY2 → USB Serial/JTAG
    - FS_PHY1 → OTG_FS

Tokom projektovanja ploče USB hub je greškom povezan na:

- **FS_PHY2 (GPIO26/GPIO27)**

umesto na:

- **FS_PHY1 (GPIO24/GPIO25)**

Zbog toga integrisani USB-Serial/JTAG kontroler nije bio povezan sa USB hub-om u aplikativnom režimu rada.

---

## Eksperimentalna potvrda problema

Eksperimentalno je potvrđeno da:

> Povezivanjem D+ i D− linija između headera **H1 (USB hub)** i **H3 (FS_PHY1)** dolazi do ispravnog rada USB-Serial/JTAG interfejsa i u aplikativnom režimu.

Ovim je potvrđeno da je uzrok problema pogrešno rutiranje USB PHY interfejsa, a ne softverska konfiguracija ili neispravnost mikrokontrolera.

---

## Moguća rešenja

### Rešenje 1 — Hardverski jumper između H1 i H3

Moguće je povezati:

- USB hub (**H1**)  
    sa:
- **FS_PHY1 (H3)**

pomoću jumper veze između D+ i D− linija.

Ovo omogućava korišćenje podrazumevane ESP32-P4 konfiguracije gde je FS_PHY1 namenjen USB Serial/JTAG kontroleru.

### Rešenje 2 — Promena PHY mapiranja putem eFuse konfiguracije

Alternativno, moguće je promeniti mapiranje PHY interfejsa pomoću eFuse podešavanja tako da:

- **FS_PHY2** bude povezan na USB Serial/JTAG kontroler
- **FS_PHY1** bude povezan na OTG_FS

Na taj način postojeće hardversko povezivanje ostaje neizmenjeno.

---

## Napomena o reviziji čipa

Korišćeni mikrokontroler:

- ESP32-P4

Za ispravan rad neophodno je u svakom projektu eksplicitno podesiti odgovarajuću reviziju čipa.

---

## Alternativno rešenje (eksterni USB-Serial adapter)

Pre identifikacije uzroka problema korišćen je eksterni USB-Serial adapter:

- CP2102 USB-Serial konverter

### Povezivanje

- TX (adapter) → GPIO38 (RX na ESP32-P4)
- RX (adapter) → GPIO37 (TX na ESP32-P4)

Ova konfiguracija omogućava standardnu UART komunikaciju nezavisno od integrisanog USB interfejsa.

---

## Test funkcionalnosti

Za testiranje je korišćen jednostavan “Hello World” program sa periodičnim restartom.

### Rezultat ispisa

```text
Hello world!
This is esp32p4 chip with 2 CPU core(s), silicon revision v1.3, 2MB external flash
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

- Problem nije bio u firmware-u niti u ESP32-P4 mikrokontroleru
- Uzrok problema je:
    - USB hub povezan na FS_PHY2 umesto na FS_PHY1
- Podrazumevana ESP32-P4 konfiguracija:
    - FS_PHY1 → USB Serial/JTAG
    - FS_PHY2 → OTG_FS
- Eksperimentalno potvrđeno:
    - Povezivanje H1 i H3 omogućava ispravan rad USB-Serial/JTAG interfejsa
- Moguća rešenja:
    - Hardverski jumper između H1 i H3
    - Promena PHY mapiranja putem eFuse konfiguracije
- Eksterni CP2102 adapter:
    - Validan workaround za UART komunikaciju
- Obavezna konfiguracija:
    - Ispravno podešavanje revizije čipa (v1.3)