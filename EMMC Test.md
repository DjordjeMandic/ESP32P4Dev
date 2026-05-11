# Testiranje eMMC komunikacije (ESP32-P4 + 8GTF4R)

## Opis sistema

Testirana je komunikacija između mikrokontrolera **ESP32-P4** i eMMC memorijskog čipa **8GTF4R** korišćenjem SDMMC interfejsa u okviru ESP-IDF okruženja.

Komunikacija je realizovana u **8-bitnom režimu magistrale**, što je u skladu sa preporukama za eMMC uređaje radi postizanja maksimalnog protoka.

---

## Podržani režimi brzine

Prema dokumentaciji ESP-IDF SDMMC host drajvera, podržani su sledeći režimi rada:

- **Default Speed (20 MHz)**
    - SD kartice: 1-bit / 4-bit
    - eMMC (3.3 V): 1-bit / 4-bit / 8-bit
- **High Speed (40 MHz)**
    - SD kartice: 1-bit / 4-bit
    - eMMC (3.3 V): 1-bit / 4-bit / 8-bit
- **Napredni režimi (1.8 V, samo za SD kartice)**:
    - SDR104 (200 MHz)
    - SDR50 (100 MHz)
    - DDR50 (50 MHz)
- **High Speed DDR (40 MHz)**
    - eMMC (3.3 V), 4-bit režim

Za konkretan slučaj eMMC čipa na 1.8 V, praktično su primenljivi:

- **Default Speed (20 MHz)**
- **High Speed (40 MHz)**

---

## Definicije frekvencija (sd_protocol_types.h)

```cpp
#define SDMMC_FREQ_DEFAULT 20000 /*!< SD/MMC Default speed (limited by clock divider) */
#define SDMMC_FREQ_HIGHSPEED 40000 /*!< SD High speed (limited by clock divider) */
#define SDMMC_FREQ_PROBING 400 /*!< SD/MMC probing speed */
#define SDMMC_FREQ_52M 52000 /*!< MMC 52MHz speed */
#define SDMMC_FREQ_26M 26000 /*!< MMC 26MHz speed */
#define SDMMC_FREQ_DDR50 50000 /*!< MMC 50MHz speed */
#define SDMMC_FREQ_SDR50 100000 /*!< MMC 100MHz speed */
#define SDMMC_FREQ_SDR104 200000 /*!< MMC 200MHz speed */
```

---

## Hardverska konfiguracija

Za pravilan rad eMMC modula neophodno je:

- Korišćenje **8-bitne magistrale** (može i manje)
- Ispravno mapiranje pinova

Primer konfiguracije:

```cpp
CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED=y
CONFIG_EXAMPLE_EMMC_BUS_WIDTH_8=y
CONFIG_EXAMPLE_PIN_CMD=39
CONFIG_EXAMPLE_PIN_CLK=40
CONFIG_EXAMPLE_PIN_D0=45
CONFIG_EXAMPLE_PIN_D1=46
CONFIG_EXAMPLE_PIN_D2=47
CONFIG_EXAMPLE_PIN_D3=43
CONFIG_EXAMPLE_PIN_D4=44
CONFIG_EXAMPLE_PIN_D5=42
CONFIG_EXAMPLE_PIN_D6=41
CONFIG_EXAMPLE_PIN_D7=48
```

---

## Napajanje SDMMC PHY (kritičan korak)

Pre inicijalizacije eMMC interfejsa obavezno je omogućiti napajanje SDMMC PHY sloja putem LDO regulatora:

```cpp
#include "esp_ldo_regulator.h"

esp_ldo_channel_handle_t ldo_phy = NULL;
esp_ldo_channel_config_t ldo_phy_cfg = {
    .chan_id = 4,
    .voltage_mv = 1800,
};

ESP_ERROR_CHECK(esp_ldo_acquire_channel(&ldo_phy_cfg, &ldo_phy));
```

**Napomena:**  
Bez ovog koraka GPIO MUX za SDMMC interfejs nema napajanje, što dovodi do neispravnog rada tj. potpune nemogućnosti komunikacije.

---

## Rezultati testiranja frekvencija

### 1. 52 MHz (SDMMC_FREQ_52M)

- **Status:** Nestabilno
- Zaključak: **Ne koristiti ovu frekvenciju**

---

### 2. 26 MHz (SDMMC_FREQ_26M)

```cpp
host.max_freq_khz = SDMMC_FREQ_26M;
```

- Realno ostvarena brzina: **~22.86 MHz**
- Zaključak: Frekvencija se ne postiže u potpunosti.

---

### 3. 20 MHz (SDMMC_FREQ_DEFAULT)

```cpp
host.max_freq_khz = SDMMC_FREQ_DEFAULT;
```

- Ostvarena brzina: **20.00 MHz**
- Stabilnost: **Potpuno stabilno**
- Sve operacije (mount, read/write, rename) uspešne

---

### 4. 40 MHz (SDMMC_FREQ_HIGHSPEED)

```cpp
host.max_freq_khz = SDMMC_FREQ_HIGHSPEED;
```

- Ostvarena brzina: **40.00 MHz**
- Stabilnost: **Potpuno stabilno**
- Sve operacije uspešne

---

## Funkcionalni test

U svim stabilnim režimima (20 MHz i 40 MHz) uspešno su izvršene sledeće operacije:

- Montiranje fajl sistema
- Kreiranje fajla
- Upis podataka
- Preimenovanje fajla
- Čitanje podataka
- Demontiranje uređaja

Primer sadržaja:

```
Read from file: 'Hello 8GTF4R!'
```

---

## Zaključak

- **Preporučene radne frekvencije:**
    - `SDMMC_FREQ_DEFAULT` (20 MHz) – maksimalna stabilnost
    - `SDMMC_FREQ_HIGHSPEED` (40 MHz) – optimalan odnos brzine i stabilnosti
- **Ne preporučuje se:**
    - 52 MHz – nestabilan rad
    - 26 MHz – ne postiže nominalnu frekvenciju
- **Obavezni uslovi za rad:**
    - Aktivacija LDO kanala 4 na 1.8 V pre inicijalizacije

---

## Napomena

Nakon završetka rada sa eMMC uređajem, moguće je osloboditi LDO kanal 4 radi smanjenja potrošnje:

Detalji:  
[https://docs.espressif.com/projects/esp-idf/en/stable/esp32p4/api-reference/peripherals/ldo_regulator.html](https://docs.espressif.com/projects/esp-idf/en/stable/esp32p4/api-reference/peripherals/ldo_regulator.html)

---

### Ispis tokom testiranja na serijskom portu

##### Frekvencija 26MHz
```cpp
host.max_freq_khz = SDMMC_FREQ_26M;
```

```
I (298) main_task: Calling app_main()
I (298) example: Starting eMMC example
I (298) example: Enabling SDMMC phy power on LDO channel 4
I (298) example: SDMMC phy power enabled
I (298) example: Initializing eMMC
I (308) example: Using SDMMC peripheral
I (308) example: Mounting filesystem
I (358) example: Filesystem mounted
Name: 8GTF4R
Type: MMC
Speed: 22.86 MHz (limit: 26.00 MHz)
Size: 7456MB
CSD: ver=3, sector_size=512, capacity=15269888 read_bl_len=9
EXT CSD: bus_width=8
I (368) example: Opening file /eMMC/hello.txt
I (378) example: File written
I (378) example: Renaming file /eMMC/hello.txt to /eMMC/foo.txt
I (378) example: Reading file /eMMC/foo.txt
I (378) example: Read from file: 'Hello 8GTF4R!'
I (388) example: Card unmounted
I (388) main_task: Returned from app_main()
```

##### Frekvencija 20MHz
```cpp
sdmmc_host_t host = SDMMC_HOST_DEFAULT();
host.max_freq_khz = SDMMC_FREQ_DEFAULT; // 20MHz
```

```
I (298) main_task: Calling app_main()
I (298) example: Starting eMMC example
I (298) example: Enabling SDMMC phy power on LDO channel 4
I (298) example: SDMMC phy power enabled
I (298) example: Initializing eMMC
I (308) example: Using SDMMC peripheral
I (308) example: Mounting filesystem
I (358) example: Filesystem mounted
Name: 8GTF4R
Type: MMC
Speed: 20.00 MHz (limit: 20.00 MHz)
Size: 7456MB
CSD: ver=3, sector_size=512, capacity=15269888 read_bl_len=9
EXT CSD: bus_width=8
I (368) example: Opening file /eMMC/hello.txt
I (368) example: File written
I (388) example: Renaming file /eMMC/hello.txt to /eMMC/foo.txt
I (388) example: Reading file /eMMC/foo.txt
I (388) example: Read from file: 'Hello 8GTF4R!'
I (388) example: Card unmounted
I (388) main_task: Returned from app_main()
```

##### Frekvencija 40MHz
```cpp
sdmmc_host_t host = SDMMC_HOST_DEFAULT();
host.max_freq_khz = SDMMC_FREQ_HIGHSPEED; // 40MHz
```

```
I (298) main_task: Calling app_main()
I (298) example: Starting eMMC example
I (298) example: Enabling SDMMC phy power on LDO channel 4
I (298) example: SDMMC phy power enabled
I (298) example: Initializing eMMC
I (308) example: Using SDMMC peripheral
I (308) example: Mounting filesystem
I (358) example: Filesystem mounted
Name: 8GTF4R
Type: MMC
Speed: 40.00 MHz (limit: 40.00 MHz)
Size: 7456MB
CSD: ver=3, sector_size=512, capacity=15269888 read_bl_len=9
EXT CSD: bus_width=8
I (368) example: Opening file /eMMC/hello.txt
I (378) example: File written
I (388) example: Renaming file /eMMC/hello.txt to /eMMC/foo.txt
I (388) example: Reading file /eMMC/foo.txt
I (388) example: Read from file: 'Hello 8GTF4R!'
I (388) example: Card unmounted
I (388) main_task: Returned from app_main()
```

---

### Ispis example programa

```
I (299) main_task: Calling app_main()
I (299) example: Starting eMMC example
I (299) example: Enabling LDO channel 4 at 1.8V
I (299) example: LDO channel 4 enabled at 1.8V
I (299) example: Initializing eMMC
I (309) example: Using SDMMC peripheral
I (309) example: Mounting filesystem
I (359) example: Filesystem mounted
Name: 8GTF4R
Type: MMC
Speed: 40.00 MHz (limit: 40.00 MHz)
Size: 7456MB
CSD: ver=3, sector_size=512, capacity=15269888 read_bl_len=9
EXT CSD: bus_width=8
I (369) example: Opening file /eMMC/hello.txt
I (379) example: File written
I (389) example: Renaming file /eMMC/hello.txt to /eMMC/foo.txt
I (389) example: Reading file /eMMC/foo.txt
I (389) example: Read from file: 'Hello 8GTF4R!'
I (389) example: Card unmounted
I (389) example: LDO channel 4 released
I (399) main_task: Returned from app_main()
```