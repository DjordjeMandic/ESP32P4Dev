U14 dc dc converter je pogresan stavljen (tokom asemblaze ploce), umesto TPSM33620S5Q (5V) je postavljen TPSM33620S3Q (3.3V). Oba regulatora su adjustable. Kada je feedback pin direktno vezan na izlaz, koristi se fiksna vrednost napona koja je navedena. Kada feedback ide preko razdelnika napona onda je ocekivani napon na feedback pinu 1V. Za 5V je potrebno odseci feedback vod sa druge strane ploce ispod U14 i onda se moze staviti 12.4k (ili 12k) otpornik izmedju feedback pina (gornji pad otpornika R2) i mase, a drugi otpornik od 49.9k (ili 51k) izmedju feedback pina i izlaza regulatora.

Ovaj fajl definise progress testiranja razvojne ploce sa esp32p4 i esp32c6

Integrisani USB/SERIAL/JTAG kontroler ne radi za P4 u aplikativnom rezimu, radi flashing preko usb u boot modu. Kada se spoje data pinovi na H1 i H3 onda radi. Potencijalni problem moze biti hardverske prirode samog ESP32P4 ili softverske konfiguracije u projektu. Resenje nije nadjeno osim eksperimentalnog resenja za povezivanje H1 i H3. Mozda je i do nekompatibilnosti sa USB-Hubom na ploci.

Adapter ploca za konektore za display ima invertovan konektor za flat kabl. Na ploci broj 5 je to primeceno prilikom povezivanja ekrana kada je flat kabl poceo da gori. Potencijalno ostecenje mipi dsi interfejsa u ESP32P4 i ostecenje ekrana. Na ploci je stavljen ? za mipi dsi konektor da se testira ponovo uz novi ekran i opposite side flat kabl. Ekran koji je koriscen prilikom testa kada je kabl izgoreo je obelezen sa X markerom.

# ESP32-C6

Hello-world example iz esp-idf radi, flashovan direktno preko ch340g na samoj ploci koji ide preko usb-huba. Izlaz na 115200 baud.

# ESP32-P4

Revizija cipa je v1.3 i to je potrebno podesiti u menuconfig u espidf-u. (SDK configuration editor)

Stavljanjem u boot rezim (drzanjem boot dugmeta prilikom reseta na EN dugmetu), esp32p4 se enumerise na usb hubu i usb bootloader je aktivan. Istovremeno je aktivan i bootloader na uart0 sa konfiguracijom (TX=37 RX=38). Flashing je moguce uraditi preko usb bootloadera preko ttyACM* device-a ili preko externog usb-uart adaptera povezanog na pinove 37/38.

U menuconfig-u espidf-a stdio na uart0 radi preko pinova 37/38, ali usb-serial preko integrisanog usb-a od esp32p4 ne radi, esp32p4 nece da se enumerise preko usb-hub-a u aplikativnom modu. Jedini nacin da se enumerise u aplikativnom radu jeste da se spoje data pinovi headera h3 i h1. Tada usb-serial radi. (postoje 2 rezima rada, aplikativni firmware i boot mode ROM)


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

Integrisani USB/SERIAL/JTAG kontroler na ESP32-P4 trenutno ne radi pravilno u aplikativnom režimu firmware-a.

## Trenutno stanje

### Radi

- flashing preko USB bootloader moda
- flashing preko UART0
- USB enumeracija u ROM boot modu

### Ne radi

- USB enumeracija u aplikativnom režimu firmware-a preko integrisanog USB interfejsa

---

## Eksperimentalno rešenje

Utvrđeno je da USB-serial funkcionalnost proradi kada se povežu:

- `H1` data pinovi
- `H3` data pinovi

Nakon toga:

- USB uređaj se pravilno enumeriše
- USB-serial komunikacija funkcioniše

---

## Potencijalni uzroci problema

Mogući uzroci uključuju:

- hardverski problem ESP32-P4
- softversku konfiguraciju projekta
- problem kompatibilnosti sa USB Hub-om na ploči

Trenutno trajno rešenje nije pronađeno.

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

# Problem sa USB Enumeracijom u Aplikativnom Režimu

U ESP-IDF `menuconfig` podešavanju:

- `stdio` preko UART0 radi pravilno
- komunikacija preko GPIO37/38 funkcioniše

Međutim:

- USB-serial preko integrisanog USB interfejsa ESP32-P4 ne radi u aplikativnom firmware režimu
- uređaj se ne enumeriše preko USB Hub-a

---

# Eksperimentalno Funkcionalno Rešenje

Jedini trenutno potvrđeni način rada USB-serial funkcionalnosti u aplikativnom režimu jeste:

- povezivanje data linija headera `H1` i `H3`

Nakon toga:

- USB enumeracija funkcioniše
- USB-serial komunikacija radi pravilno

---

# Napomena

ESP32-P4 poseduje dva različita režima rada:

|Režim|Opis|
|---|---|
|ROM Boot Mode|Interni bootloader|
|Application Firmware Mode|Korisnički firmware|
