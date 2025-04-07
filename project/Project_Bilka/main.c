/*
 * Project_Bilka.c
 *
 * Created: 3/31/2025 11:59:26
 * Author : Student
 */ 
/************************************************************************/
/* INCLUDE   
uvolnenie 100 kohm 
stlacenie 400 ohm                                                            */
/*****/
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "uart/uart.h"
#include "makra.h"
/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/

FILE uart_str = FDEV_SETUP_STREAM(printCHAR, NULL, _FDEV_SETUP_RW);//soubor pro stdout

char text[]="label"; //= 'l''a''b''e''l''/0'

/************************************************************************/
/* PROTOTYPES                                                           */
/************************************************************************/

void board_init();


// Inicializácia ADC
void adc_init() {
	// Nastavenie referen?ného napätia na 1.5V (interný referen?ný zdroj)
	ADMUX = (1 << REFS1);  // REFS1 = 1 (VREF = 1.5V), REFS0 = 0 (VREF = 1.5V)
	
	// Povolenie ADC a nastavenie preddelova?a na 128
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

// ?ítanie hodnoty z ADC
uint16_t adc_read(uint8_t channel) {
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);  // Výber kanálu
	ADCSRA |= (1 << ADSC);  // Spustenie konverzie
	while (ADCSRA & (1 << ADSC));  // ?akaj na skon?enie konverzie
	return ADC;
}

void board_init(){
	
	
	UART_init(38400); //baudrate 38400b/s
	
	UCSR1B |= (1 << RXCIE1);// UART receive interrupt enable
	//sbi(UCSR1B,RXCIE1);
	stdout = &uart_str; //standartní výstup/std output
	
}


int main(void) {
	// Inicializácia UART
	board_init();
	adc_init();   // Inicializuj ADC

	char buffer[32];
	uint16_t adc_value;

	UART_SendChar(27);//escape
	UART_SendString("[2J");//clear and home
	UART_SendChar(27);//escape
	UART_SendString("[0;32;40m");//barva pozadi a textu
	
	// Vytlac uvítaciu správu
	printf("-------------------------------------\n\r");
	printf("Vitaj v programe pre snimanie obsadenia pomocou FSR senzora!\n\r");

	while (1) {
	// cítanie hodnoty z FSR senzora na kanáli 0 (PF0)
		adc_value = adc_read(0);  // Kanál 0 je priradený k PF0 (ADC0)
		snprintf(buffer, sizeof(buffer), "ADC Value: %u\n\r", adc_value);  // Formátuj hodnotu
		UART_SendString(buffer);  // Pošli cez UART
		
		
		
		// Skontroluj, ?i je hodnota väcšia ako 100 (miesto je obsadené)
		if (adc_value > 100) {
			UART_SendString("Miesto je obsadene!\n\r");
		} else {
			UART_SendString("Miesto nie je obsadene.\n\r");
		}
		
		_delay_ms(500);  // Po?kaj 500ms pred ?alším ?ítaním
	}


	return 0;
}