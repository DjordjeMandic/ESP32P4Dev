Na ploči su implementirana dva ulazna kanala (Illumination i Ignition) koji su preko serijskog otpornika od 10 kΩ i Zener diode za zaštitu od prenapona povezani na GPIO pinove ESP32-P4 (GPIO26 i GPIO27).

Za testiranje funkcionalnosti ulaza koristi se projekat `gpio_ign_ilum_test`, koji omogućava očitavanje logičkog stanja ovih pinova i time indirektnu proveru prisustva napona na terminalnom bloku pored konektora H3. Neophodno je u `sdkconfig` fajlu podesiti ciljnu reviziju čipa na v1.x.

Testiranje se vrši dovođenjem napona na odgovarajući kontakt terminalnog bloka (tipično iz laboratorijskog izvora). Merenjem je utvrđeno ponašanje napona na GPIO pinu u zavisnosti od ulaznog napona:

- Bez aktivnog pull-down otpornika:
    - 3.3 V → 2.1 V (GPIO)
    - 32.3 V → 3.1 V (GPIO)
- Sa aktivnim internim pull-down otpornikom:
    - 3.3 V → 1.9 V (GPIO)
    - 32.3 V → 3.1 V (GPIO)        

(Nižа vrednost predstavlja napon na GPIO pinu, dok je viša vrednost ulazni napon na terminalu.)

Prema specifikaciji, minimalni napon za garantovano detektovanje logičke jedinice na GPIO ulazu iznosi približno 2.5 V. Ova vrednost se postiže kada ulazni napon na terminalnom bloku dostigne oko 8 V ili više.

Iako se u praksi prelazni prag detekcije nalazi oko ~1.8 V (tipična vrednost), rad u tom opsegu nije garantovan.

S obzirom da se u realnoj primeni očekuje automobilsko napajanje u opsegu od 12 V do 36 V, može se zaključiti:
- U 12 V sistemima, naponi ispod ~9 V ne obezbeđuju pouzdanu detekciju logičke jedinice
- Za stabilan i specifikacijom pokriven rad, ulaz treba da bude iznad ~8 V

Primer ispisa sa serijskog interfejsa tokom testiranja:

```
Illumination: 1
Ignition: 0

Illumination: 0
Ignition: 0

Illumination: 0
Ignition: 1
```

Na osnovu rezultata može se zaključiti da ulazni sklop ispravno detektuje prisustvo napona u predviđenom radnom opsegu.