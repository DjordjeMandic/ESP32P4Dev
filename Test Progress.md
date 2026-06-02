# Test Progress – ESP32-P4 + ESP32-C6 Razvojna Ploča

Ovaj dokument definiše trenutno stanje testiranja, poznate hardverske probleme i specifičnosti razvojne ploče bazirane na ESP32-P4 i ESP32-C6 mikrokontrolerima.

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
    - sa druge strane ploče
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

# USB / SERIAL / JTAG – Ploča Broj 5

Na ploči broj 5 slučajno je programiran eFuse:

- `USB_PHY_SEL`

Programiranjem ovog eFuse-a izvršena je trajna zamena mapiranja između:

- USB Serial/JTAG PHY interfejsa
- USB OTG FS PHY interfejsa

Promena je trajna i ne može se poništiti.

---

## Posledice

Nakon promene mapiranja:

- USB Serial/JTAG interfejs više nije povezan na predviđeni USB konektor
- USB Serial/JTAG signalne linije završavaju na ulazima za:
    - IGNITION
    - ILLUMINATION

Zbog toga je integrisani USB Serial/JTAG kontroler praktično neupotrebljiv na ploči broj 5 za:

- flashing firmware-a
- USB serial komunikaciju
- JTAG debugging

---

## Header H3

Nakon promene mapiranja, header `H3` povezan je sa USB OTG FS interfejsom.

Može se koristiti za:

- USB OTG FS funkcionalnost
- korišćenje odgovarajućih GPIO pinova u aplikaciji

---

## USB 2.0 Interfejs

Problem ne utiče na USB 2.0 interfejs ESP32-P4 koji je povezan direktno na USB Hub.

Test projekat:

- `tusb_serial_device_test`

uspešno radi i pravilno se enumeriše preko USB Hub-a i na ploči broj 5.

Time je potvrđena ispravnost:

- USB Hub-a
- USB 2.0 PHY interfejsa
- USB 2.0 signalnih linija
- USB konektora

---

## Flashing Ploče Broj 5

Firmware se trenutno upisuje korišćenjem:

- USB-to-Serial TTL adaptera (`CP2102`)
- UART0 interfejsa

### UART0 konfiguracija

|Signal|GPIO|
|---|---|
|TX|GPIO37|
|RX|GPIO38|

Za više detalja pogledati dokumentaciju za USB.

---

# MIPI DSI Display Problem

Adapter ploča za display konektore poseduje invertovan FPC konektor.

Problem je primećen na ploči broj 5 tokom prvih testova kada je korišćen neodgovarajući flat kabl.

Prilikom povezivanja došlo je do termičkog oštećenja FPC kabla.

Naknadnim testiranjem potvrđeno je da:

- ploča broj 5 funkcioniše ispravno    
- korišćeni displej funkcioniše ispravno
- nije došlo do trajnog oštećenja MIPI DSI interfejsa

---

## Status hardvera

| Oznaka | Značenje                                                       |
| ------ | -------------------------------------------------------------- |
| ?      | Potrebna dodatna provera                                       |
| X      | Ranije korišćen u incidentu ili naknadno potvrđen kao ispravan |

---

# ESP32-C6 Status

## Funkcionalnost

`hello_world` example iz ESP-IDF-a uspešno radi.

### Način flashovanja

- direktno preko CH340G USB-UART kontrolera
- preko integrisanog USB Hub-a na ploči

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

U tom režimu aktivni su:

- USB bootloader
- UART0 bootloader

---

# Flashing Opcije

Firmware je moguće flashovati:

## Preko USB bootloadera

- preko `ttyACM*` uređaja

## Preko UART0 interfejsa

- korišćenjem eksternog USB-UART adaptera
- GPIO37 (TX)
- GPIO38 (RX)

---

# Napomena

ESP32-P4 poseduje dva različita režima rada:

|Režim|Opis|
|---|---|
|ROM Boot Mode|Interni bootloader|
|Application Firmware Mode|Korisnički firmware|

Ponašanje USB interfejsa može se razlikovati između ova dva režima rada, posebno kod ploče broj 5 zbog trajno programiranog eFuse-a `USB_PHY_SEL`.