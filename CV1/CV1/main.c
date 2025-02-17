/*
 * CV1.c
 *
 * Created: 2/17/2025 13:57:24
 * Author : Student
 */ 

#include <avr/io.h>
#include <avr/delay.h>
// Makra pro manipulaci s bity
#define setbit(port, bit)   ((port) |= (1 << (bit)))  // Nastavit bit na 1
#define clearbit(port, bit) ((port) &= ~(1 << (bit))) // Nastavit bit na 0
#define xorbit(port, bit)   ((port) ^= (1 << (bit)))  // Zm?nit stav bitu
int main(void)
{
		/* Replace with your application code */
	// binary notation
	//DDRB =  0b01000000;
	//PORTB = 0b00000000;
	// equivalent hex notation
	//DDRG = 0x02;
	//PORTG = 0x02;
	// equivalent decimal notation
	//DDRG = 2;
	//PORTG = 2;
	while(1)
	 {
	//PORTB = 0b01000000;
	//_delay_loop_2(0xFFFF);
	//PORTB = 0b00000000;
	//_delay_loop_2(0xFFFF);
	 
    DDRB = 0b01110000;  // Nastav� piny 4, 5, 6 jako v�stupy (pro LED na bitech 4, 5, 6)

	    // ** Bin�rn� z�pis pro zapnut� LED diod **

	    // LED0 (bit 4) zapnut�
	    PORTB = 0b00010000;  // Zapnut� LED0 na pinu 4
	    _delay_loop_2(0xFFFF);  // Zpo�d?n�

	    // LED1 (bit 5) zapnut�
	    PORTB = 0b00100000;  // Zapnut� LED1 na pinu 5
	    _delay_loop_2(0xFFFF);  // Zpo�d?n�

	    // LED2 (bit 6) zapnut�
	    PORTB = 0b01000000;  // Zapnut� LED2 na pinu 6
	    _delay_loop_2(0xFFFF);  // Zpo�d?n�

	    // ** Dekadick� z�pis pro zapnut� LED diod **

	    PORTB = 16;  // Zapnut� LED0 (bit 4) v dekadick�m z�pisu
	    _delay_loop_2(0xFFFF);  // Zpo�d?n�
	    PORTB = 32;  // Zapnut� LED1 (bit 5) v dekadick�m z�pisu
	    _delay_loop_2(0xFFFF);  // Zpo�d?n�
	    PORTB = 64;  // Zapnut� LED2 (bit 6) v dekadick�m z�pisu
	    _delay_loop_2(0xFFFF);  // Zpo�d?n�

	    // ** Hexadecim�ln� z�pis pro zapnut� LED diod **

	    PORTB = 0x10;  // Zapnut� LED0 (bit 4) v hexadecim�ln�m z�pisu
	    _delay_loop_2(0xFFFF);  // Zpo�d?n�
	    PORTB = 0x20;  // Zapnut� LED1 (bit 5) v hexadecim�ln�m z�pisu
	    _delay_loop_2(0xFFFF);  // Zpo�d?n�
	    PORTB = 0x40;  // Zapnut� LED2 (bit 6) v hexadecim�ln�m z�pisu
	    _delay_loop_2(0xFFFF);  // Zpo�d?n�

	    // ** Pou�it� maker pro zapnut�/vypnut� LED diod **
	    
	    setbit(PORTB, 4);  // Zapnut� LED0 pomoc� makra setbit (bit 4)
	    _delay_loop_2(0xFFFF);  // Zpo�d?n�
	    setbit(PORTB, 5);  // Zapnut� LED1 pomoc� makra setbit (bit 5)
	    _delay_loop_2(0xFFFF);  // Zpo�d?n�
	    setbit(PORTB, 6);  // Zapnut� LED2 pomoc� makra setbit (bit 6)
	    _delay_loop_2(0xFFFF);  // Zpo�d?n�

	    clearbit(PORTB, 4);  // Vypnut� LED0 pomoc� makra clearbit (bit 4)
	    _delay_loop_2(0xFFFF);  // Zpo�d?n�
	    clearbit(PORTB, 5);  // Vypnut� LED1 pomoc� makra clearbit (bit 5)
	    _delay_loop_2(0xFFFF);  // Zpo�d?n�
	    clearbit(PORTB, 6);  // Vypnut� LED2 pomoc� makra clearbit (bit 6)
	    _delay_loop_2(0xFFFF);  // Zpo�d?n�

	    // ** Pou�it� XOR pro zm?nu stavu LED diod **
	    xorbit(PORTB, 4);  // Zm?na stavu LED0 pomoc� makra xorbit (bit 4)
	    _delay_loop_2(0xFFFF);  // Zpo�d?n�
	    xorbit(PORTB, 5);  // Zm?na stavu LED1 pomoc� makra xorbit (bit 5)
	    _delay_loop_2(0xFFFF);  // Zpo�d?n�
	    xorbit(PORTB, 6);  // Zm?na stavu LED2 pomoc� makra xorbit (bit 6)
	    _delay_loop_2(0xFFFF);  // Zpo�d?n�
    }
	 
	 
}

