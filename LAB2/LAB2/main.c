/*
 * LAB2.c
 *
 * Created: 2/24/2025 12:34:11
 * Author : Student
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "makra.h"  // P?ipojen� makra pro ovl�d�n� LED a bitov�ch operac�

#define BUTTON_PIN    PD2  // Pin pro tla?�tko (INT0)



void USART_Init(uint16_t baud) {
	uint16_t ubrr = F_CPU / 16 / baud - 1;
	UBRR1H = (uint8_t)(ubrr >> 8);
	UBRR1L = (uint8_t)ubrr;
	UCSR1B = (1 << RXEN1) | (1 << TXEN1);
	UCSR1C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART_Transmit(uint8_t data) {
	while (!(UCSR1A & (1 << UDRE1))) { }
	UDR1 = data;
}

void USART_SendString(const char *text) {
	while (*text) {
		USART_Transmit(*text++);
	}
}

/* Funkce pro p?ijet� jednoho znaku */
uint8_t USART_Receive(void) {
	while (!(UCSR1A & (1 << RXC1))) { }
	return UDR1;
}

/* Funkce pro vymaz�n� obrazovky */
void clear_screen(void) {
	USART_SendString("\033[2J");  // Escape sekvence pro vymaz�n� obrazovky
}

/* Funkce pro nastaven� ?erven� barvy textu */
void set_red(void) {
	USART_SendString("\033[31m");  // Escape sekvence pro ?ervenou barvu
}

/* Funkce pro resetov�n� barvy textu */
void reset_color(void) {
	USART_SendString("\033[0m");  // Escape sekvence pro resetov�n� barvy
}

void LED_Blink(void) {
	DDRB = 0b01110000;  // Nastaven� pin? 4, 5, 6 jako v�stupy
	setbit(PORTB, 4);
	_delay_ms(500);
	clearbit(PORTB, 4);
	_delay_ms(500);
	setbit(PORTB, 5);
	_delay_ms(500);
	clearbit(PORTB, 5);
	_delay_ms(500);
	setbit(PORTB, 6);
	_delay_ms(500);
	clearbit(PORTB, 6);
	_delay_ms(500);
}

// P?eru�en� pro detekci stisku tla?�tka
ISR(INT0_vect) {
	USART_Transmit('1');  // Odesl�n� znaku '1' p?i stisku tla?�tka
}

int main(void) {
	// Inicializace USART s baudrate 38400
	USART_Init(38400);

	// Povoleni globalnich preruseni
	sei();

	// Nastaven� tla?�tka jako vstup
	DDRD &= ~(1 << BUTTON_PIN);  // Nastaven� pin PD2 jako vstup
	PORTD |= (1 << BUTTON_PIN);  // Aktivace pull-up rezistoru

	// Povoleni p?eru�en� pro tla?�tko (INT0)
	EIMSK |= (1 << INT0);    // Povolen� p?eru�en� INT0
	EICRA |= (1 << ISC01);   // Nastaven� p?eru�en� na pokles sign�lu (aktivn� low)
	// Nastaven� ?erven� barvy pro cel� text
	set_red();

	// Zobrazen� MENU s popisem voleb
	USART_SendString("MENU:\r\n");
	USART_SendString("0: Ukonceni programu\r\n");
	USART_SendString("1: Vypis male abecedy\r\n");
	USART_SendString("2: Vypis velke abecedy\r\n");
	USART_SendString("3: Blikani LED 3x\r\n");
	USART_SendString("4: Vymazat obrazovku a zobrazit MENU\r\n");

	while (1) {
		uint8_t recv = USART_Receive();  // Cek�me na p?�kaz od u�ivatele

		// Reakce na vstupy
		switch (recv) {
			case '0':
			// Ukon?en� programu
			USART_SendString("Program ukoncen.\r\n");
			return 0;
			
			case '1':
			// Vypis male abecedy
			for (char c = 'a'; c <= 'z'; c++) {
				USART_Transmit(c);
			}
			USART_Transmit('\r');
			USART_Transmit('\n');
			break;
			
			case '2':
			// Vypis velke abecedy
			for (char c = 'A'; c <= 'Z'; c++) {
				USART_Transmit(c);
			}
			USART_Transmit('\r');
			USART_Transmit('\n');
			break;
			
			case '3':
			// Blik�n� LED
			USART_SendString("Blikani LED.\r\n");
			LED_Blink();
			break;
			
			case '4':
			// Vymaz�n� obrazovky a zobrazen� menu
			clear_screen();
			USART_SendString("MENU:\r\n");
			USART_SendString("0: Ukonceni programu\r\n");
			USART_SendString("1: Vypis male abecedy\r\n");
			USART_SendString("2: Vypis velke abecedy\r\n");
			USART_SendString("3: Blikani LED 3x\r\n");
			USART_SendString("4: Vymazat obrazovku a zobrazit MENU\r\n");
			break;

			default:
			// O�et?en� nezn�m�ho vstupu
			USART_SendString("Neznamy vstup, zadejte hodnotu 0-4.\r\n");
			break;
		}
	}

	return 0;
}