# Test Progress / TODO – ESP32-P4 + ESP32-C6 Razvojna Ploča

Ovaj dokument definiše trenutno stanje testiranja i poznate hardverske i softverske probleme razvojne ploče bazirane na ESP32-P4 i ESP32-C6 mikrokontrolerima.

---

# Poznati Hardverski Problemi

## U14 DC/DC Konverter – Pogrešna Verzija Komponente

Tokom asemblaze ploče postavljen je pogrešan DC/DC regulator.

### Očekivana komponenta

- `TPSM33620S5Q` (5V verzija)

### Stvarno postavljena komponenta

- `TPSM33620S3Q` (3.3V verzija)

Obe verzije regulatora podržavaju podešavanje izlaznog napona preko feedback mreže.

---

## Način rada feedback pina

Kada je feedback pin direktno povezan na izlaz regulatora:

- koristi se interna fiksna vrednost izlaznog napona regulatora    

Kada se koristi otpornik-delitelj:

- očekivani napon na feedback pinu iznosi:
    - **1V**

---

## Potrebna modifikacija za 5V izlaz

Da bi regulator radio na 5V potrebno je:

1. Prekinuti feedback vod:
    - sa druge strane ploče,
    - ispod U14 regulatora
2. Dodati otpornike:

|Veza|Vrednost|
|---|---|
|FB → GND|12.4kΩ (ili 12kΩ)|
|FB → VOUT|49.9kΩ (ili 51kΩ)|

### Fizičke tačke povezivanja

Feedback pin je dostupan preko:

- gornjeg pada otpornika `R2`

---

# USB / SERIAL / JTAG Problemi – ESP32-P4

Na ploči broj 5 integrisani USB Serial/JTAG (USJ) kontroler nije funkcionalan zbog greškom programiranog eFuse-a:

- `USB_PHY_SEL`

Zbog toga:

- USB flashing preko USJ nije moguć
- USB serial komunikacija preko USJ nije dostupna
- JTAG debugging preko USJ nije dostupan

Problem utiče isključivo na integrisani USB Serial/JTAG kontroler i ne utiče na USB 2.0 interfejs povezan direktno na USB hub.

Test projekat:

- `tusb_serial_device_test`

uspešno radi i pravilno se enumeriše preko USB 2.0 interfejsa i na ploči broj 5, što potvrđuje da su:

- USB hub
- USB 2.0 PHY
- USB 2.0 signalne linije

ispravni.

Firmware na ploču broj 5 trenutno se upisuje korišćenjem:

- USB-to-Serial TTL adaptera (`CP2102`)
- UART0 interfejsa (`GPIO37` i `GPIO38`)

Za vise detalja procitati `esp32p4 usb.md`

---

# MIPI DSI Display Problem

Adapter ploča za display konektore poseduje invertovan FPC konektor.

Problem je primećen na ploči broj 5 prilikom povezivanja displeja, kada je flat kabl počeo termički da se oštećuje.

---

## Potencijalne posledice

Postoji mogućnost:

- oštećenja MIPI DSI PHY interfejsa na ESP32-P4
- oštećenja displeja

---

## Status hardvera

Nakon incidenta:

|Oznaka|Značenje|
|---|---|
|`?`|Sumnjiv MIPI DSI konektor / ploča|
|`X`|Potencijalno oštećen ekran|

---

## Planirano

Potrebno je ponoviti testiranje sa:

- novim displejem
- FPC kablom sa kontaktima na suprotnim stranama (_opposite side flat cable_)

---

# ESP32-C6 Status

## Funkcionalnost

`hello_world` example iz ESP-IDF-a uspešno radi.

### Način flashovanja

- direktno preko CH340G USB-UART kontrolera, preko integrisanog USB Hub-a na ploči

### Serijski izlaz

|Parametar|Vrednost|
|---|---|
|Baudrate|115200|

---

# ESP32-P4 Status

## Revizija čipa

Korišćeni ESP32-P4 je:

- revizija `v1.3`    

Ovo je potrebno pravilno podesiti u:

- `SDK Configuration Editor`

u okviru ESP-IDF projekta.

---

# Boot Režim

ESP32-P4 ulazi u boot režim:

- držanjem `BOOT` dugmeta
- pritiskom na `EN/RESET`

U tom režimu:

- aktivan je USB bootloader
- aktivan je UART0 bootloader

---

## UART0 konfiguracija

|Signal|GPIO|
|---|---|
|TX|GPIO37|
|RX|GPIO38|

---

# Flashing Opcije

Firmware je moguće flashovati:

## Preko USB bootloadera

- preko `ttyACM*` uređaja

## Preko eksternog USB-UART adaptera

- povezivanjem na:
    - GPIO37 (TX)
    - GPIO38 (RX)

---

# Napomena

ESP32-P4 poseduje dva različita režima rada:

|Režim|Opis|
|---|---|
|ROM Boot Mode|Interni bootloader|
|Application Firmware Mode|Korisnički firmware|
