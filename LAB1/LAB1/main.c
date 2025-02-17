/*
 * LAB1.c
 *
 * Created: 02.02.2020 9:01:38
 * Author : Ondra
 */ 

/************************************************************************/
/* INCLUDE                                                              */
/************************************************************************/
#include <avr/io.h>
#include <util/delay.h>
#include "libs/libprintfuart.h"
#include <stdio.h>
/************************************************************************/
/* DEFINES                                                              */
/************************************************************************/

//Definice konstanty
#define HODNOTAX 2
#define UPPER_CASE 1
#define NORMAL_CASE 0

#define DIRECTION_UP 1
#define DIRECTION_DOWN 0


//Definice direktivy pro odcitanie
#define ODECET

// F_CPU definovano primo v projektu!!! Debug->Properties->Toolchain->Symbols

/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/

int a = 10;
unsigned char b = 255;
unsigned char c = 255;
unsigned int sum;
int z;

int value=200;
char str[50]="hodnota=";
char field[52];

//musime vytvorit soubor pro STDOUT
FILE uart_str = FDEV_SETUP_STREAM(printCHAR, NULL, _FDEV_SETUP_RW);

/************************************************************************/
/* PROTOTYPES                                                           */
/************************************************************************/

void board_init();

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
void board_init(){
	UART_init(38400); //nastaveni rychlosti UARTu, 38400b/s
	stdout = &uart_str; //presmerovani STDOUT
}

// Funkce pro napln?n� pole p�smeny a-z, A-Z
void generateField(int case_type) {
	int i;
	
	if (case_type == UPPER_CASE) {
		for (i = 0; i < 26; i++) {
			field[i] = 'A' + i;  // Velk� p�smena
		}
		} else if (case_type == NORMAL_CASE) {
		for (i = 0; i < 26; i++) {
			field[i] = 'a' + i;  // Mal� p�smena
		}
		} else {
		printf("PROGRAM ERROR: Invalid case type.\n");
		exit(1); // Ukon?� program, pokud je zad�n neplatn� case_type
	}
	
	// Dopl?�me velk� p�smena (A-Z) na pozice 26 a� 51
	for (i = 0; i < 26; i++) {
		field[26 + i] = 'A' + i;  // Velk� p�smena
	}
}

// Funkce pro zm?nu mal�ch p�smen na velk� a naopak
void capsLetters(int case_type) {
	int i;
	
	if (case_type == UPPER_CASE) {
		// P?evedeme v�echna p�smena na velk�
		for (i = 0; i < 52; i++) {
			if (field[i] >= 'a' && field[i] <= 'z') {
				field[i] = field[i] - 'a' + 'A';
			}
		}
		} else if (case_type == NORMAL_CASE) {
		// P?evedeme v�echna p�smena na mal�
		for (i = 0; i < 52; i++) {
			if (field[i] >= 'A' && field[i] <= 'Z') {
				field[i] = field[i] - 'A' + 'a';
			}
		}
		} else {
		printf("PROGRAM ERROR: Invalid case type.\n");
		exit(1); // Ukon?� program, pokud je zad�n neplatn� case_type
	}
}

// Funkce pro vyps�n� pole p�smen na obrazovku
void printField(int direction) {
	int i;
	
	// Zkontrolujeme, zda je sm?r v�pisu spr�vn�
	if (direction == DIRECTION_UP) {
		for (i = 0; i < 52; i++) {
			printf("%c ", field[i]);
		}
		printf("\n");
		} else if (direction == DIRECTION_DOWN) {
		for (i = 51; i >= 0; i--) {
			printf("%c ", field[i]);
		}
		printf("\n");
		} else {
		printf("PROGRAM ERROR: Invalid direction.\n");
		exit(1); // Ukon?� program, pokud je sm?r neplatn�
	}
}

int main(void)
{ 	

	board_init();
	_delay_ms(1000);
	printf("Hello word\n\r");
	
	#ifdef ODECET
	a -= HODNOTAX;  // Ode?teme hodnotu konstanty HODNOTAX od a
	#endif
	printf("Hodnota a = %d\n\r", a);  // Vyp�eme novou hodnotu prom?nn� a
	
	sum= (unsigned int)(b+c);
	printf("Suma b+c=%d\n\r",sum);
	
	z = (24 >> 3) -1 & 0x2;

	printf("Hodnota Z= %d\n\r",z);
	
	char valueStr[20];
	snprintf(valueStr, sizeof(valueStr),"%d",value);
	strcat(str,valueStr);
	
	 // V�pis v�sledku
	 printf("Vysledek pomoci string.h a stdlib.h: %s\n\r", str);
	 
	 strcpy(str, "hodnota=");
	 sprintf(str+strlen(str), "%d",value);
	 
	 // V�pis v�sledku
	 printf("Vysledek pomoci sprintf: %s\n\r", str);
	 
	  // Testov�n� funk?nosti
	  int case_type = UPPER_CASE; // Volba pro velk� p�smena
	  int direction = DIRECTION_UP; // Volba pro vzestupn� v�pis
	  
	  // Generujeme pole p�smen
	  generateField(case_type);
	  
	  // Vyprintujeme pole p�smen vzestupn?
	  printf("Pole pismen vzestupne:\n\r");
	  printField(direction);
	  
	  // Zm?n�me p�smena z velk�ch na mal�
	  capsLetters(NORMAL_CASE);
	  
	  // Vyprintujeme pole p�smen sestupn?
	  printf("Pole pismen sestupne:\n\r");
	  printField(DIRECTION_DOWN);
	 
	  int number = 42;
	  
	  int *ptr = &number;
	  
	  printf("Hodnota promenne 'number' je: %d\n\r", *ptr);  // Dereference ukazatele pro z�sk�n� hodnoty
	  printf("Adresa promenne 'number' je: %p\n\r", (void*)ptr);  // Adresa v pam?ti (ukazatel)

    _delay_ms(1000);
	int i=0;
    while (1) 
    {
	_delay_ms(10000);
	i++;
	printf("Test x = %d \n\r", i);
    }
	
	
	
	
}

