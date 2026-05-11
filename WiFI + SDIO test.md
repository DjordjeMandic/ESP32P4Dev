Testiranje Wi-Fi funkcionalnosti izvršeno je flashovanjem ESPHome firmware-a na ESP32-C6 koristeći integrisani CH340G USB-to-serial adapter na ploči, koji je povezan na USB hub. Na ovaj način omogućeno je testiranje kompletnog Wi-Fi frontend-a, uključujući podršku za 802.15.4 i Bluetooth.

Za verifikaciju veze između ESP32-P4 i ESP32-C6 korišćeni su ESPHome `binary_sensor` GPIO ulazi na C6 strani. Praćeni su pinovi GPIO18, GPIO19, GPIO20, GPIO21, GPIO22 i GPIO23. Sa druge strane, ESP32-P4 generiše test signal sekvencijalnim togglovanjem GPIO pinova 19, 18, 14, 15, 16 i 17 (tim redosledom).

Ovakav pristup omogućava validaciju fizičke konekcije između dva kontrolera na modulu, koja se koristi i za SDIO komunikaciju.

Flashovanje ESPHome firmware-a na ESP32-C6 izvršeno je sa sledećom konfiguracijom:

```yaml
esphome:
  name: esp32p4test
  friendly_name: esp32p4test

esp32:
  board: esp32-c6-devkitc-1
  framework:
    type: esp-idf

logger:

api:
  encryption:
    key: "key"

ota:
  - platform: esphome
    password: "password"

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

ap:
  ssid: "Esp32P4Test Fallback Hotspot"
  password: "password"

captive_portal:

web_server:
  port: 80

binary_sensor:
  - platform: gpio
    name: "GPIO19 (P4) -> GPIO18 (C6)"
    pin: GPIO18
  - platform: gpio
    name: "GPIO18 (P4) -> GPIO19 (C6)"
    pin: GPIO19
  - platform: gpio
    name: "GPIO14 (P4) -> GPIO20 (C6)"
    pin: GPIO20
  - platform: gpio
    name: "GPIO15 (P4) -> GPIO21 (C6)"
    pin: GPIO21
  - platform: gpio
    name: "GPIO16 (P4) -> GPIO22 (C6)"
    pin: GPIO22
  - platform: gpio
    name: "GPIO17 (P4) -> GPIO23 (C6)"
    pin: GPIO23
```

Na ESP32-P4 koristi se jednostavan test projekat (`gpio_c6_test`) koji sekvencijalno toggluje navedene GPIO pinove sa pauzom od 1 sekunde između promene stanja. Neophodno je podesiti ciljnu reviziju čipa na v1.x.

Rezultati očitani iz ESPHome log-a potvrđuju očekivano ponašanje:

```
[19:54:20.989][S][binary_sensor]: 'GPIO19 (P4) -> GPIO18 (C6)' >> ON
[19:54:21.977][S][binary_sensor]: 'GPIO19 (P4) -> GPIO18 (C6)' >> OFF
[19:54:22.989][S][binary_sensor]: 'GPIO18 (P4) -> GPIO19 (C6)' >> ON
[19:54:23.984][S][binary_sensor]: 'GPIO18 (P4) -> GPIO19 (C6)' >> OFF
[19:54:24.986][S][binary_sensor]: 'GPIO14 (P4) -> GPIO20 (C6)' >> ON
[19:54:25.978][S][binary_sensor]: 'GPIO14 (P4) -> GPIO20 (C6)' >> OFF
[19:54:26.987][S][binary_sensor]: 'GPIO15 (P4) -> GPIO21 (C6)' >> ON
[19:54:27.977][S][binary_sensor]: 'GPIO15 (P4) -> GPIO21 (C6)' >> OFF
[19:54:28.986][S][binary_sensor]: 'GPIO16 (P4) -> GPIO22 (C6)' >> ON
[19:54:29.977][S][binary_sensor]: 'GPIO16 (P4) -> GPIO22 (C6)' >> OFF
[19:54:30.983][S][binary_sensor]: 'GPIO17 (P4) -> GPIO23 (C6)' >> ON
[19:54:31.976][S][binary_sensor]: 'GPIO17 (P4) -> GPIO23 (C6)' >> OFF
```

Istovremeno, serijski izlaz sa ESP32-P4 potvrđuje sekvencijalno togglovanje pinova:

```
GPIO19 toggle
ON
OFF
GPIO18 toggle
ON
OFF
GPIO14 toggle
ON
OFF
GPIO15 toggle
ON
OFF
GPIO16 toggle
ON
OFF
GPIO17 toggle
ON
OFF
```

Na osnovu dobijenih rezultata može se zaključiti da je GPIO veza između ESP32-P4 i ESP32-C6 ispravna, čime je potvrđena i osnovna hardverska povezanost potrebna za SDIO komunikaciju.