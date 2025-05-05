# Project FSR_Čidlo

**Autor**: Tomáš Bilka  
**Vytvorené**: 31.3.2025  

Program pre snímanie obsadenia miesta pomocou FSR senzora a bezdrôtovú komunikáciu.

---

## Obsah

- [Popis projektu](#popis-projektu)
- [Použité knižnice](#použité-knižnice)
- [Hardvérové požiadavky](#hardvérové-požiadavky)
- [Inštalácia a spustenie](#inštalácia-a-spustenie)
- [Architektúra programu](#architektúra-programu)
- [Príklad výstupu](#výstup-z-konzole)

---

## Popis projektu

Tento projekt realizuje:
- Snímanie tlaku pomocou FSR senzora cez ADC.
- Odosielanie nameraných hodnôt cez bezdrôtovú sieť.
- Spracovanie a zobrazovanie prijatých hodnôt cez UART terminál.
- Indikáciu obsadenia miesta na základe prahovej hodnoty.

## Použité knižnice
Pre základ LWM siete bol použitý základ od Pána Ph.D. Krajsu. 
- `avr/io.h`, `util/delay.h`, `avr/interrupt.h` - Nízkoúrovňové ovládanie MCU
- `stdlib.h`, `stdio.h`, `string.h`, `math.h` - Štandardné knižnice C
- Projektové knižnice:
  - `config.h`, `hal.h`, `phy.h`, `sys.h`, `nwk.h`, `sysTimer.h`
  - `halBoard.h`, `halUart.h`, `makra.h`, `main.h`
  - `uart/uart.h`

## Hardvérové požiadavky

- **Mikrokontrolér**: AVR (napr. ATmega)
- **Senzor**: FSR (Force Sensitive Resistor)
- **Pripojenie k PC**: UART/USB
- **Napájanie**: 3.3V alebo 5V podľa MCU
- **FSR**: vlastnosti:
- stlačenie: 400 Ohm
- Uvolnenie: 100 KOhm
- Referenčné napetie: 1,5 V
## Inštalácia a spustenie

1. Nahrajte projekt do AVR mikrokontroléra (napr. cez Atmel Studio).
2. Pripojte FSR senzor na ADC kanál 0.
3. Spustite UART terminál s nastaveniami:
   - Baudrate: 38400
   - 8 data bits, no parity, 1 stop bit
4. Sledujte hodnoty ADC a hlášky o obsadení.

## Architektúra programu

**Hlavné komponenty:**
- Operácia | Funkcia | Účel
- Inicializácia UART | board_init(), HAL_UartInit() | Nastavenie komunikácie so sériovým portom.
- Inicializácia ADC | adc_init() | Meranie analógovej hodnoty z FSR.
- Inicializácia siete | appInit() | Nastavenie bezdrôtovej siete.
- Čítanie dát | adc_read() | Získanie hodnoty zo senzora.
- Odoslanie dát | appSendData(), NWK_DataReq() | Prenos nameraných údajov cez sieť.
- Prijatie dát | appDataInd() | Spracovanie prijatých správ zo siete.



## Výstup z konzole:
![image](https://github.com/user-attachments/assets/4458dd52-bb0d-42c7-a41a-f2ed53e76222)

## Graf rozdielu hodnôt:
![image](https://github.com/user-attachments/assets/dd068fa2-c434-4c3f-98f4-3f16761f0212)
.
## Možnosť doimplementovať:
- IDLE stav
- Filtrovanie na základe rozdielov hodnôt senzora určovať človeka vs krabica
---

