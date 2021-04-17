# SiloVibrationMeter
Urządzenie do pomiaru wibracji powierzchniowych konstrukcji silosów

Urządzenie przeznaczone jest do pomiarów wibracji powierzchni silosów w konstrukcjach stalowych. Wibracje mierzone są w trzech osiach, za pomocą akcelerometrów i przekazywane do centralnego komputera.
Za kanał komunikacyjny wybrano UDP.

### Charakterystyka
  * Podstawa pomiarowa: trzy akcelerometry **ADXL001**, zasilanymi ze źródłeł napięcia referencyjnego **ADR3433ARJZ**.
  * Budowa jednego toru pomiarowego: programowalne źródło napięcia odniesienia **MCP4921** dla wzmacniacza Rail-to-Rail **MCP6S2** + **dwa** wspólne napięcia referencyjne (**REF02**).
  * Jednostka centralna: **dsPIC33FJ128MC706A** (16bit, architektura MIPS).
  * Komunikacja: kontroler ethernetowy **ENC28J60** z wbudowanym transformatorem separującym. Prędkośc komunikacji: **10Mbps**.
  * Obsługiwane protokoły: **UDP**, **IP**, **ARP**, **DHCP**, **ICMP**.
  * Metoda akwizycji danych: typu **Ping-Pong**, wykorzystująca wbudowany kontroler **DMA** (równoległa budowa nowej paczki pomiarów i wysyłanie już wcześniej zbudowane).

### Zdjęcia

Widok głównego okna aplikacji zbierającej dane z wielu urządzeń pomiarowych:
![](images/gui.png)

Prototyp pierwszy:
![](images/proto1.jpg)

Prototyp drugi:
![](images/proto2_1.jpg)
![](images/proto2_2.jpg)
![](images/proto2_3.jpg)
![](images/proto2_4.jpg)
![](images/proto2_5.jpg)
![](images/proto2_6.jpg)
![](images/proto2_7.jpg)
![](images/proto2_8.jpg)
![](images/proto2_9.jpg)

Os pionowa:
![](images/yaxis.jpg)

Testy prototypu drugiego:
![](images/test1.jpg)
![](images/test2.jpg)
![](images/test3.jpg)
![](images/test4.jpg)
![](images/test5.jpg)
![](images/test6.jpg)

Ustawienia bitów w środowisku MPLAB:
![](mplab_bits.png)


### Schematy
Blok CPU:
![Blok CPU](docs/main1.png)
Interfejs sieciowy:
![Interfejs sieciowy](docs/main2.png)
Zasilanie:
![Zasilanie](docs/main3.png)
Akcelerometry i napięcie referencyjne:
![Akcelerometry i napięcie referencyjne](docs/main4.png)
Programowalne wzmacniacze:
![Programowalne wzmacniacze](docs/main5.png)
Konwerter poziomów 3V3 -> 5V:
![Konwerter](docs/main6.png)

Wersje PDF:
  * [Płyta główna - schemat](main_schemat.pdf)
  * [Płyta główna - PCB](docs/main_PCB.pdf)
  * [Moduł osi Z - schemat](osZ_inv.pdf)
  * [Moduł osi Z - PCB](osZ_inv_PCB.pdf )

