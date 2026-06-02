## Opis

Adapter ploča služi za povezivanje displeja sa glavnom razvojnom pločom korišćenjem dva različita FPC konektora.

Tokom testiranja utvrđeno je nekoliko grešaka u šemi i rasporedu signala koje je potrebno ispraviti kako bi adapter radio ispravno.

---

# FPC Kabl

Konektor `U1` ima invertovan raspored pinova u odnosu na očekivanu orijentaciju displeja.

Zbog toga je neophodno koristiti:

- **opposite side FPC (inverted) flat kabl**

Korišćenje standardnog FPC kabla može dovesti do pogrešnog povezivanja signala i potencijalnog oštećenja kabla ili hardvera.

---

# Modifikacija U1 Konektora

Pin 17 konektora `U1` potrebno je povezati na masu.

### Potrebna izmena

|Signal|Akcija|
|---|---|
|U1 Pin 17|Povezati na GND|

Na ploči je prisutan deo ground plane-a u neposrednoj blizini ovog pina, tako da je za realizaciju dovoljan:

- jedan solder bridge između pina 17 i mase

---

# Modifikacije Signala

## J1 – Signal `$1N2153`

Signal:

- `$1N2153`

potrebno je povezati sa signalom:

- `VCI_EN`

---

## R4

Otpornik `R4` nije potreban u postojećoj konfiguraciji.

Potrebna izmena:

- kratko spojiti (`0 Ω`)

---

## R2

Otpornik `R2` nije potreban u postojećoj konfiguraciji.

Potrebna izmena:

- kratko spojiti (`0 Ω`)

---

## R9 / CSX Signal

Signal:

- `CSX`

na poziciji `R9`

potrebno je povezati sa signalom:

- `VCI_EN`

na konektoru `J1`.

Potrebna izmena:

- kratko spojiti CSX i VCI_EN mreže

---

# Rezime Potrebnih Izmena

|Lokacija|Izmena|
|---|---|
|U1 Pin 17|Povezati na GND|
|J1 `$1N2153`|Povezati na `VCI_EN`|
|R2|Kratkospojiti|
|R4|Kratkospojiti|
|R9 (CSX)|Povezati na `VCI_EN`|
|FPC kabl|Koristiti opposite side (inverted) FPC kabl|

---

# Status

Navedene modifikacije potrebno je izvršiti pre korišćenja adapter ploče sa displejem.