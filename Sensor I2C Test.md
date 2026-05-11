# Testiranje I2C magistrale (ESP32-P4)

## Cilj testiranja

Cilj ovog testa je verifikacija ispravnosti **I2C magistrale**, odnosno provera da li se povezane komponente pravilno odazivaju i omogućavaju komunikaciju.

Test je izvršen korišćenjem `i2c_tools` primera iz ESP-IDF okruženja. Potrebno je podesiti chip revision na v1.x.

---

## Pokretanje alata

Nakon pokretanja aplikacije, dostupan je interaktivni CLI interfejs sa sledećim osnovnim komandama:

```bash
I (297) main_task: Calling app_main()

 ==============================================================
 |             Steps to Use i2c-tools                         |
 |                                                            |
 |  1. Try 'help', check all supported commands               |
 |  2. Try 'i2cconfig' to configure your I2C bus              |
 |  3. Try 'i2cdetect' to scan devices on the bus             |
 |  4. Try 'i2cget' to get the content of specific register   |
 |  5. Try 'i2cset' to set the value of specific register     |
 |  6. Try 'i2cdump' to dump all the register (Experiment)    |
 |                                                            |
 ==============================================================


Type 'help' to get the list of commands.
Use UP/DOWN arrows to navigate through command history.
Press TAB when typing command name to auto-complete.
I (437) main_task: Returned from app_main()
i2c-tools> 
```

---

## Konfiguracija magistrale

Pre bilo kakve komunikacije, potrebno je konfigurisati I2C pinove:

```bash
i2c-tools> i2cconfig --scl=32 --sda=31
```

- **SCL**: GPIO32
- **SDA**: GPIO31

---

## Detekcija uređaja na magistrali

Skeniranje magistrale izvršeno je komandom:

```bash
i2c-tools> i2cdetect
```

Rezultat:

```
i2c-tools> i2cdetect
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
50: -- 51 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- -- -- 6a -- -- -- -- -- 
70: -- -- -- -- -- -- -- -- -- -- -- -- -- -- 7e --
```

### Identifikovani uređaji

- **0x51** – RTC (Real-Time Clock)
- **0x6A** – akcelerometar

Prisustvo očekivanih adresa potvrđuje da su uređaji fizički povezani i odgovaraju na I2C magistrali.

---

## Test čitanja registara (akcelerometar)

Za akcelerometar je korišćen identifikacioni registar `WHO_AM_I`:

- Adresa registra: `0x00`
- Očekivana vrednost: `0x05`

Komanda:

```bash
i2c-tools> i2cget -c 0x6a -r 0x00
```

Rezultat:

```
0x05
```

**Zaključak:**  
Dobijena vrednost odgovara datasheet specifikaciji, čime je potvrđena ispravna komunikacija sa akcelerometrom.

---

## Test čitanja i upisa (RTC)

Za RTC je testiran RAM registar:

- Adresa registra: `0x03`
- Tip: Read/Write

### Koraci testiranja

1. Čitanje početne vrednosti:

```bash
i2c-tools> i2cget -c 0x51 -r 0x03
```

Rezultat:

```
0x00
```

2. Upis nove vrednosti:

```bash
i2c-tools> i2cset -c 0x51 -r 0x03 0x55
```

3. Ponovno čitanje:

```bash
i2c-tools> i2cget -c 0x51 -r 0x03
```

Rezultat:

```
0x55
```

**Zaključak:**  
Uspešan upis i čitanje potvrđuju da RTC uređaj ispravno funkcioniše na I2C magistrali.
