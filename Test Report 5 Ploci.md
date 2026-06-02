# ESP32-P4 Test Report (5 Boards)

## Test Overview

Testovi obuhvataju sledeće funkcionalnosti:

- Power rails: 1.8V, 3.3V, 5V
- CAN
- eMMC
- WiFi (SDIO – ESP32-C6)
- I2C (Accelerometer + RTC)
- Display (DSI + I2C touch)
- CN3 UART
- CN4 UART
- 12V / 24V Inputs (Illumination & Ignition)

---

## Legend

|Status|Meaning|
|---|---|
|PASS|Test uspešan|
|FAIL|Test neuspešan|
|N/A|Nije testirano|

---

## Board #1

| Test                        | Result | Notes |
| --------------------------- | ------ | ----- |
| 1.8V Rail                   | PASS   | 1.79V |
| 3.3V Rail                   | PASS   | 3.35V |
| 5V Rail                     | PASS   | 5.24V |
| CAN                         | PASS   |       |
| eMMC                        | PASS   | 40MHz |
| WiFi (SDIO)                 | PASS   |       |
| I2C (Accelerometer + RTC)   | PASS   |       |
| Display (DSI)               | PASS   |       |
| Touch (I2C)                 | PASS   |       |
| CN3 UART                    | PASS   |       |
| CN4 UART                    | N/A    |       |
| Illumination Input (12/24V) | PASS   |       |
| Ignition Input (12/24V)     | PASS   |       |

---

## Board #2

| Test                        | Result | Notes |
| --------------------------- | ------ | ----- |
| 1.8V Rail                   | PASS   | 1.8V  |
| 3.3V Rail                   | PASS   | 3.34V |
| 5V Rail                     | PASS   | 5.27V |
| CAN                         | PASS   |       |
| eMMC                        | PASS   | 40MHz |
| WiFi (SDIO)                 | PASS   |       |
| I2C (Accelerometer + RTC)   | PASS   |       |
| Display (DSI)               | PASS   |       |
| Touch (I2C)                 | PASS   |       |
| CN3 UART                    | PASS   |       |
| CN4 UART                    | N/A    |       |
| Illumination Input (12/24V) | PASS   |       |
| Ignition Input (12/24V)     | PASS   |       |

---

## Board #3

| Test                        | Result | Notes |
| --------------------------- | ------ | ----- |
| 1.8V Rail                   | PASS   | 1.78V |
| 3.3V Rail                   | PASS   | 3.32V |
| 5V Rail                     | PASS   | 5.27V |
| CAN                         | PASS   |       |
| eMMC                        | PASS   | 40MHz |
| WiFi (SDIO)                 | PASS   |       |
| I2C (Accelerometer + RTC)   | PASS   |       |
| Display (DSI)               | PASS   |       |
| Touch (I2C)                 | PASS   |       |
| CN3 UART                    | PASS   |       |
| CN4 UART                    | N/A    |       |
| Illumination Input (12/24V) | PASS   |       |
| Ignition Input (12/24V)     | PASS   |       |

---

## Board #4

| Test                        | Result | Notes |
| --------------------------- | ------ | ----- |
| 1.8V Rail                   | PASS   | 1.8V  |
| 3.3V Rail                   | PASS   | 3.35V |
| 5V Rail                     | PASS   | 5.27V |
| CAN                         | PASS   |       |
| eMMC                        | PASS   | 40MHz |
| WiFi (SDIO)                 | PASS   |       |
| I2C (Accelerometer + RTC)   | PASS   |       |
| Display (DSI)               | PASS   |       |
| Touch (I2C)                 | PASS   |       |
| CN3 UART                    | PASS   |       |
| CN4 UART                    | N/A    |       |
| Illumination Input (12/24V) | PASS   |       |
| Ignition Input (12/24V)     | PASS   |       |

---

## Board #5

| Test                        | Result | Notes                                       |
| --------------------------- | ------ | ------------------------------------------- |
| 1.8V Rail                   | PASS   | 1.8V                                        |
| 3.3V Rail                   | PASS   | 3.32V                                       |
| 5V Rail                     | PASS   | 5.27V                                       |
| CAN                         | PASS   |                                             |
| eMMC                        | PASS   | 40MHz                                       |
| WiFi (SDIO)                 | PASS   |                                             |
| I2C (Accelerometer + RTC)   | PASS   |                                             |
| Display (DSI)               | PASS   | Invertovan pinout upotrebljen na ovoj ploči |
| Touch (I2C)                 | PASS   | Invertovan pinout upotrebljen na ovoj ploči |
| CN3 UART                    | PASS   |                                             |
| CN4 UART                    | N/A    |                                             |
| Illumination Input (12/24V) | PASS   |                                             |
| Ignition Input (12/24V)     | PASS   |                                             |

---

## Summary (Optional)

| Board | Passed Tests | Failed Tests | Notes                                                                                                                                 |
| ----- | ------------ | ------------ | ------------------------------------------------------------------------------------------------------------------------------------- |
| #1    | 12           |              |                                                                                                                                       |
| #2    | 12           |              |                                                                                                                                       |
| #3    | 12           |              |                                                                                                                                       |
| #4    | 12           |              |                                                                                                                                       |
| #5    | 12           | 1            | Pogledati DSI Test Result Note<br>USJ (usb-serial-jtag) je na ploci ne upotrebljiv zbog greskom spaljenog efuse-a. Pogledati USB docs |
