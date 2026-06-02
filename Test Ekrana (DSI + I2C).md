# Test Izveštaj – DSI Displej (CO5300 + CST820B)

## Opis

Testiranje DSI displeja izvršeno je korišćenjem projekta `co5300_test`, zajedno sa alatom `i2c_tools` za proveru prisustva touch kontrolera na I2C magistrali.

Korišćeni modul sastoji se od:

- `CO5300` display kontrolera
- `CST820B` I2C touch kontrolera

Touch kontroler povezan je na sledeće GPIO pinove:

|Signal|GPIO|
|---|---|
|SDA|GPIO23|
|SCL|GPIO22|

---

## Napajanje

Pre početka bilo kakve inicijalizacije displeja neophodno je omogućiti:

- **LDO kanal 3**
- izlazni napon: **2500 mV**

Ovaj korak je obavezan i identičan je proceduri koja se koristi prilikom inicijalizacije eMMC memorije.

---

## Softverske Komponente

Za podršku displeju korišćena je ESP-IDF komponenta:

- `espressif/esp_lcd_co5300`

Tokom testiranja korišćeni su:

- `co5300_test` – test inicijalizacije i prikaza slike
- `i2c_tools` – provera prisustva CST820B kontrolera na I2C magistrali

---

## DSI Test

Test je uspešno završen sa sledećom konfiguracijom:

|Parametar|Vrednost|
|---|---|
|Rezolucija|466 × 466|
|Format slike|RGB888|
|Dubina boje|24-bit|

Prilikom korišćenja rezolucije 466×466 potrebno je ukloniti proveru podržane rezolucije iz fajla:

`espressif__esp_lcd_co5300/esp_lcd_co5300_mipi.c`

Potrebno je ukloniti sledeći kod:

```cpp
if (vendor_config->mipi_config.dpi_config->video_timing.h_size *
    vendor_config->mipi_config.dpi_config->video_timing.v_size % 8 != 0) {
    ESP_GOTO_ON_FALSE(false, ESP_ERR_NOT_SUPPORTED, err, TAG,
                      "unsupported resolution");
}
```

Nakon uklanjanja navedene provere displej radi korektno u RGB888 režimu.

---

## Incident Tokom Testiranja

Tokom jednog od ranijih pokušaja testiranja korišćen je neodgovarajući FPC kabl usled greške na adapter ploči koja koristi invertovan FPC konektor.

U tom slučaju:

- displej nije odgovarao na inicijalizaciju,
- MIPI DSI komunikacija nije uspostavljena,
- aplikacija je ostala blokirana čekajući odgovor displeja,
- aktiviran je `Task Watchdog Timer (task_wdt)`.

Nakon dodatne provere utvrđeno je da nije došlo do trajnog oštećenja.

Ploča broj 5 i displej koji je obeležen oznakom `X` uspešno su ponovo testirani i funkcionalni su.

---

## Test I2C Touch Kontrolera

Prisustvo CST820B touch kontrolera provereno je korišćenjem alata `i2c_tools`.

Očekivana I2C adresa uređaja:

- `0x15`
- ili `0x14`

Konfiguracija magistrale:

```text
i2c-tools> i2cconfig --scl=22 --sda=23
i2c-tools> i2cdetect
```

Rezultat:

```text
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
10: -- -- -- -- -- 15 -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
```

Detektovana je adresa `0x15`, čime je potvrđena ispravna komunikacija sa CST820B kontrolerom.

---

## Zaključak

Testiranje displeja uspešno je završeno.

Potvrđena je funkcionalnost:

- MIPI DSI interfejsa
- CO5300 display kontrolera
- RGB888 (24-bit) režima rada
- CST820B I2C touch kontrolera
- I2C komunikacije na GPIO23/GPIO22

DSI displej i touch interfejs rade ispravno na testiranoj hardverskoj platformi.