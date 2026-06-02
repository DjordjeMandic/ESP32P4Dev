# USB Arhitektura i Korišćenje USB Portova — ESP32-P4 Razvojna Ploča

## Pregled USB Arhitekture

ESP32-P4 poseduje ukupno tri USB interfejsa:

- **2× USB 1.1**
- **1× USB 2.0**

Na razvojnoj ploči USB portovi su povezani preko USB hub-a i imaju različite namene.

---

# USB 2.0 Port

USB 2.0 interfejs je povezan direktno na USB hub i koristi se isključivo kroz aplikativni firmware.

Ovaj port:

- nije namenjen za flashing firmware-a preko integrisanog USB Serial/JTAG kontrolera
- nije automatski aktivan
- mora biti inicijalizovan u kodu

Tipično se koristi kroz:

- TinyUSB

Primeri upotrebe:

- USB HID uređaji
- USB CDC
- USB MSC
- custom USB protokoli

## Status testiranja USB 2.0 interfejsa

Test projekat:

- `tusb_serial_device_test`

uspešno radi i pravilno se enumeriše preko USB 2.0 interfejsa.

Ovo potvrđuje da su:

- USB hub
- direktna veza između USB hub-a i ESP32-P4 USB 2.0 PHY interfejsa
- USB 2.0 signalne linije na ploči

ispravni i funkcionalni.

## Napomena za ploču broj 5

Iako na ploči broj 5 integrisani USB Serial/JTAG kontroler ne radi zbog pogrešno programiranog eFuse-a (sam ESP32P4 je obeležen sa crnom tačkom) , USB 2.0 interfejs i dalje radi normalno jer je povezan direktno na USB hub i nije zavisan od USB Serial/JTAG kontrolera.

Na ploči broj 5 test projekat:

- `tusb_serial_device_test`

uspešno je upisan koristeći:

- USB-to-Serial TTL adapter `CP2102`

nakon čega se uređaj pravilno enumerisao preko USB 2.0 interfejsa.

---

# USB Serial/JTAG (USJ) Kontroler

ESP32-P4 poseduje integrisani:

- USB Serial/JTAG kontroler
- debugging interfejs
- USB flashing interfejs
- serijski (CDC) interfejs

Ovaj kontroler koristi interni USB 1.1 PHY.

## Funkcije USJ kontrolera

USJ omogućava:

- upload firmware-a
- serial monitor komunikaciju
- JTAG debugging
- automatski boot/download režim

---

# Povezivanje USJ Kontrolera

USJ kontroler je izveden na header:

- `P4USB`
- oznaka: `H3`

Da bi integrisani USJ kontroler radio, potrebno je povezati:

- `D+`
- `D-`

sa jednim od slobodnih USB portova USB hub-a:

- `H1`
- `H2`

## Potrebna veza

Dovoljno je povezati samo:

- USB D+
- USB D-

signalne linije.

Za ovo je potreban:

- 2-pin jumper kabl

---

# Način Korišćenja

## Korišćenje integrisanog USJ kontrolera

1. Povezati `H3` (`P4USB`) na `H1` ili `H2`
2. Povezati ploču na računar preko USB hub-a
3. Koristiti:
    
    - flashing
    - serial monitor
    - debugging

kao standardni ESP32 USB Serial/JTAG uređaj.

---

# Napomena za Ploču Broj 5

## Greška sa eFuse konfiguracijom

Na ploči broj 5 greškom je programiran eFuse:

- `USB_PHY_SEL`

Zbog toga je integrisani USB Serial/JTAG kontroler trajno preusmeren na:

- ignition
- illumination

kontakte.

## Posledice

Na toj ploči:

- integrisani USJ kontroler je neupotrebljiv
- USB flashing preko USJ nije moguć
- USB serial i JTAG preko USJ nisu dostupni

---

# Korišćenje Ignition i Illumination Pinova

Iako je USJ kontroler povezan na te pinove, oni se i dalje mogu koristiti kao standardni GPIO pinovi.

---

# Flashing Firmware-a na Ploču Broj 5

Pošto USB Serial/JTAG nije dostupan, firmware se mora upisivati preko UART interfejsa.

## Potreban hardver

- USB-to-Serial TTL adapter

## Povezivanje

Adapter povezati na:

- GPIO37
- GPIO38

UART linije:

- TX
- RX

## UART0 konfiguracija

|Signal|GPIO|
|---|---|
|TX|GPIO37|
|RX|GPIO38|

## Režim rada

ESP32-P4 mora biti u:

- download/boot režimu

tokom upisivanja firmware-a.

## Spaljen EFUSE na ploči broj 5

Na ploči broj 5 slučajno je programiran eFuse `USB_PHY_SEL`, čime je izvršena zamena mapiranja između USB Serial/JTAG i USB OTG FS PHY interfejsa. Ova promena je trajna i ne može se vratiti.

Nakon promene mapiranja, USB Serial/JTAG interfejs je preusmeren na pinove koji su na ovoj razvojnoj ploči povezani sa ulazima za **IGNITION** i **ILLUMINATION**, zbog čega integrisani USB Serial/JTAG interfejs više nije praktično upotrebljiv na ploči broj 5.

Header **H3** je nakon zamene mapiranja povezan sa USB OTG FS interfejsom i može se koristiti za:

- USB OTG FS komunikaciju
- korišćenje odgovarajućih GPIO pinova u aplikaciji

Funkcionalnost ostatka sistema nije pogođena ovom promenom, osim nedostupnosti USB Serial/JTAG interfejsa na predviđenom USB konektoru.