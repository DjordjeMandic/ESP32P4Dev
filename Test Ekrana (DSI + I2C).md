# Test izveštaj – DSI ekran (CO5300 + CST820B)

## Opis

Testiranje DSI ekrana planirano je korišćenjem projekta `co5300_test`, zajedno sa alatom `i2c_tools` radi provere prisustva touch kontrolera `CST820B` na I2C magistrali.

Ekran koristi:

- `CO5300` display kontroler
- `CST820B` I2C touch kontroler

Za komunikaciju sa touch kontrolerom koriste se sledeći pinovi:

|Signal|GPIO|
|---|---|
|SDA|GPIO23|
|SCL|GPIO22|

---

## Napajanje

Pre početka bilo kakve inicijalizacije ili konfiguracije displeja potrebno je omogućiti:

- **LDO kanal 3**
- izlazni napon: **2500 mV**

Ovaj korak je obavezan i funkcionalno je sličan proceduri korišćenoj za eMMC inicijalizaciju.

---

## Softverske komponente

Za inicijalizaciju displeja korišćena je ESP-IDF komponenta:

- `espressif/esp_lcd_co5300`

Planirano je korišćenje:

- `co5300_test` — za inicijalizaciju i test prikaza slike
- `i2c_tools` — za proveru prisustva `CST820B` uređaja na I2C magistrali

---

## Trenutni status testa

Test trenutno **nije završen** i nalazi se u fazi daljeg razvoja i ispitivanja.

Prvobitno je planirana konfiguracija:

- rezolucija: **466x466**
- format slike: **RGB888 (24-bit)**

Međutim, sa navedenom konfiguracijom primećeni su problemi, zbog čega je privremeno pređeno na:

- **16-bitni format prikaza**

Dalja analiza i dodatni pokušaji konfiguracije su planirani.

---

## Primećeni problemi

Tokom jednog od testova korišćen je fizički okrenut (reverzno povezan) konektor između ploče i displeja.

U tom slučaju:

- program je očekivao odgovor od displeja,
- odgovor nije dobijen,
- inicijalizacija je ostala blokirana,
- aktiviran je `Task Watchdog Timer (task_wdt)`.

Ovakvo ponašanje je očekivano usled neispravne veze prema displeju.

---

## Zaključak

Testiranje DSI displeja još uvek nije završeno i trenutno se vodi kao:

> **TODO / Work in Progress**

Potrebno je nastaviti sa:

- analizom problema pri radu u RGB888 režimu,
- dodatnim testovima MIPI DSI konfiguracije,
- proverom stabilnosti komunikacije sa displejom i touch kontrolerom,
- validacijom rada u 16-bitnom režimu prikaza.


# Test Izveštaj – MIPI DSI Ekran (CO5300 + CST820B)

## Opis

Planirano je testiranje MIPI DSI displeja baziranog na CO5300 display kontroleru i CST820B I2C touch kontroleru na ESP32-P4 platformi.

Za inicijalno testiranje koriste se sledeći projekti:

- `co5300_test` — test prikaza slike preko MIPI DSI interfejsa
- `i2c_tools` — provera prisustva CST820B touch kontrolera na I2C magistrali

### I2C konfiguracija

|Signal|GPIO|
|---|---|
|SDA|GPIO23|
|SCL|GPIO22|

---

## Napajanje

Pre početka bilo kakve inicijalizacije ili konfiguracije displeja potrebno je omogućiti:

- **LDO kanal 3**
- izlazni napon: **2500 mV**

Ovaj korak je obavezan i funkcionalno je sličan proceduri korišćenoj za eMMC inicijalizaciju.

---

# Softverska konfiguracija

Za podršku displeju korišćena je ESP-IDF komponenta:

- `espressif/esp_lcd_co5300`

Planirana konfiguracija displeja:

|Parametar|Vrednost|
|---|---|
|Rezolucija|466 × 466|
|Format|RGB888|
|Dubina boje|24-bit|

Tokom dosadašnjeg razvoja pojavili su se problemi sa stabilnim radom u RGB888 režimu, zbog čega je privremeno prebačeno na:

- **RGB565 / 16-bit režim**

Testiranje još uvek nije završeno i postoji nekoliko dodatnih ideja i konfiguracija koje treba proveriti.

---

# Zapažanja Tokom Testiranja

Tokom jednog od pokušaja testiranja ekran je povezan preko pogrešno orijentisanog flat kabl konektora.

U tom stanju:

- firmware je pokušavao komunikaciju sa displejem,
- očekivan odgovor od displeja nije dobijen,
- došlo je do aktiviranja Task Watchdog.
    

---

# Hardverski Problem – Adapter Ploča

Utvrđeno je da adapter ploča za display konektore poseduje invertovan FPC konektor.

Greška je primećena na ploči broj 5 prilikom povezivanja displeja, kada je flat kabl počeo termički da se oštećuje (pregorevanje kabla).

## Potencijalne posledice

Postoji mogućnost sledećih oštećenja:

- oštećenje MIPI DSI PHY interfejsa na ESP32-P4
- oštećenje samog displeja

---

# Status Hardvera

Nakon incidenta:

- problematična ploča je označena znakom `?`
- korišćeni displej je označen znakom `X`

Planirano je ponovno testiranje sa:

- novim displejem
- FPC kablom sa kontaktima na suprotnim stranama (_opposite side flat cable_)

---

# Trenutni Status

## Status testa: TODO

Testiranje MIPI DSI displeja još uvek nije završeno.

Potrebno je:

- nastaviti validaciju MIPI DSI komunikacije,
- proveriti stanje ESP32-P4 MIPI DSI interfejsa,
- ponoviti test sa ispravnim FPC kablom i novim ekranom,
- nastaviti debug RGB888 konfiguracije.