
1. 🗌 Základné informácie

Názov projektu: LWM_MSSY

Autor: Krajsa

Dátum vytvorenia: 6.4.2017

Popis: Projekt pre snímanie hodnoty FSR senzora (Force Sensing Resistor) pomocou mikrokontroléra AVR, s prenášaním hodnôt cez sieť pomocou NWK protokolu a zobrazením cez UART.

2. ⚖️ Použité knižnice a hlavičkové súbory

Štandardné knižnice: stdlib.h, stdio.h, string.h, math.h, util/delay.h, avr/io.h, avr/interrupt.h

Vlastné moduly: config.h, hal.h, phy.h, sys.h, nwk.h, sysTimer.h, halBoard.h, halUart.h, main.h, makra.h, uart/uart.h

3. 🔄 Popis hlavných komponentov

3.1 Inicializácia

board_init(): Nastavenie UART (baudrate 38400 bps), povolenie UART prijímania pomocou prerušenia.

adc_init(): Inicializácia ADC s referenčným napätím 1.5V a predčelením 128.

3.2 Čítanie z ADC

adc_read(uint8_t channel): Čítanie hodnoty z daného ADC kanálu.

3.3 Sieťová komunikácia

appSendData(): Odoslanie UART bufferu cez NWK sieť.

sendFSRValue(): Špeciálne odoslanie FSR hodnoty cez NWK.

appDataConf(): Callback po odoslaní dát.

appDataInd(): Callback pri prijatí dát cez sieť (výpis na UART).

3.4 Timer

appTimerHandler(): Správca časovača pre odosielanie dát.

3.5 Stavový automat

APP_TaskHandler(): Stavový automat:

APP_STATE_INITIAL: Inicializácia siete a hardvéru.

APP_STATE_SEND_CHAR: Načítanie a odoslanie hodnoty z ADC.

APP_STATE_IDLE: Nečinný stav.

3.6 Hlavný cyklus

main():

Inicializuje systém, UART, ADC.

Vykreslí uvítaciu obrazovku.

Nekonečný cyklus spracovávajúci úložhy SYS, UART a aplikačného stavu.
![image](https://github.com/user-attachments/assets/b85158a8-0a03-47b9-aa18-63d1ec311c6c)
