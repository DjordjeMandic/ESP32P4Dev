# Testiranje CAN / TWAI interfejsa

## Cilj testiranja

Cilj ovog testa je verifikacija funkcionalnosti **CAN (TWAI) komunikacije** između više ploča, uključujući:

- ispravan prenos podataka
- stabilnost komunikacije (1Mbps)
- ponašanje u različitim režimima rada (listen-only vs normal)

---

## Test setup

Za testiranje su korišćeni ESP-IDF primeri:

- `twai_sender`
- `twai_listen_only`
### Topologija sistema

- **1 ploča** → sender
- **4 ploče** → receiver

Receiver ploče prate i loguju sav saobraćaj na magistrali.
### Fizička veza

- CAN magistrala realizovana pomoću **CAT5e parice**
- Terminacija:
    - **120Ω** između CANH i CANL na prvoj i poslednjoj ploči
    - Alternativno: moguće populisati **2×60Ω (0603)** na PCB za fiksnu terminaciju

### Transceiver

- Korišćen: **TJA1051T/3/1J**
- Silent pin:
    - povezan na **GPIO3**
    - **nije korišćen u testu** (ostavljen kao Hi-Z)
    - interni pull-down u transceiveru → normalan rad (transmitter aktivan)

---

## Konfiguracija

Za oba projekta:

```cpp
CONFIG_EXAMPLE_TWAI_TX_GPIO=1
CONFIG_EXAMPLE_TWAI_RX_GPIO=2
```

Obavezno:
- podesiti **chip revision na v1.x**

---

## Rezultati – Listen Only režim

Receiver je konfigurisan sa:

```cpp
.flags.enable_listen_only = true
```

### Posmatrano ponašanje

- Receiver uspešno prima sve poruke
- Sender prijavljuje grešku:

```
TWAI node error: 0x10
Failed to transmit message
```

### Primer ispisa (ID: 0x100)

Receiver pravilno prima sekvencijalne podatke:

```
RX: timestamp ..., 100 [8] 0 0 0 0 0 0 0 0
RX: timestamp ..., 100 [8] 1 1 1 1 1 1 1 1
RX: timestamp ..., 100 [8] 2 2 2 2 2 2 2 2
...
```

### Primer ispisa (ID: 0x7FF – heartbeat)

```
RX: timestamp ..., 7ff [8] ...
```

---

## Analiza greške

Greška:

```
0x10 → ACK error
```

Značenje:

- nijedan node na magistrali **nije poslao ACK bit**

Razlog:

- `listen_only = true` znači:
    - node **ne učestvuje aktivno na magistrali**
    - ne šalje ACK
    - ne utiče na bus (pasivno snifuje)

➡️ Dakle:
- **prenos fizički uspešan**
- ali sender **ne dobija ACK → prijavljuje grešku**

Ovo je **očekivano ponašanje**.

---

## Rešenje

Potrebno je omogućiti da bar jedan receiver:
- aktivno učestvuje na magistrali
- šalje ACK

### Izmene u konfiguraciji receiver-a

```cpp
.flags.enable_listen_only = false,
.tx_queue_depth = 5,
```

Takođe OBAVEZNO:

```cpp
#define TWAI_LISTENER_TX_GPIO CONFIG_EXAMPLE_TWAI_TX_GPIO
```

(Napomena: u originalnom example-u TX pin može biti `-1`, što onemogućava ACK)

---

## Rezultati – Normal mode

Nakon izmene:
- sender više **ne prijavljuje greške**
- komunikacija stabilna

### Ispis sender-a:

```
Sending heartbeat message: ...
Sending packet of 1000 bytes in 125 frames
```

Bez:

```
TWAI node error
```

---

## Zaključak

- CAN komunikacija je **ispravna i stabilna**
- Greška `0x10 (ACK error)`:
    - nije hardverski problem
    - već direktna posledica **listen-only režima**
- Za validan rad sistema:
    - minimum jedan node mora biti u **normal mode**
    - mora imati validan TX pin
    - mora imati TX queue (`tx_queue_depth >= 1`)
