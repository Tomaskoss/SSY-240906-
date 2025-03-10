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
#include <stdio.h>
#include <avr/interrupt.h>
#include "makra.h"
#include "uart/uart.h"
#include "Timers/Timers.h"
#include "ADC/ADC.h"
#include "I2C/I2C.h"
#include "I2C/AT30TSE758.h"
/************************************************************************/
/* DEFINES                                                              */
/************************************************************************/

#define STEP 10
// F_CPU definovano primo v projektu!!

/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/

FILE uart_str = FDEV_SETUP_STREAM(printCHAR, NULL, _FDEV_SETUP_RW);//soubor pro stdout

volatile int pruchod=0;  // 
volatile uint8_t klik=0; //
volatile char recv;
volatile uint8_t char_to_process=0;
volatile uint8_t menu_level=0;

char text[]="label"; //= 'l''a''b''e''l''/0'

volatile uint8_t duty=0; //PWM strida, nemusi byt global, ale jednodussi
volatile uint8_t auto_reg=0;

/************************************************************************/
/* PROTOTYPES                                                           */
/************************************************************************/

void board_init();
void menu1();
void menu3();
void handle_menu1();
void handle_menu3();

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
void board_init(){
	cli(); //disable interrupts
	
	 UART_init(38400); //baudrate 38400b/s
	 
	 UCSR1B |= (1 << RXCIE1);// UART receive interrupt enable
	 
	 stdout = &uart_str; //standartní výstup
	
	//LEDky
	sbi(DDRB,4);
	sbi(DDRB,5);
	sbi(DDRB,6);
	sbi(DDRE,3);
	//Všechny zapneme
	LED0ON;
	LED1ON;
	LED2ON;
	LED3ON;
	
	Timer2_fastpwm_start(100); //
	
	//ADC_Init(4,2);

	
	//Tlacitka vstup
	cbi(DDRE,4);
	//Tlacitka pull-up
	sbi(PORTE,4);
    
	sbi(EIMSK,4); //povolit preruseni INT5 - tlacitko button0 
	sbi(EICRB,ISC41);//nastupna hrana /rising edge
	sbi(EICRB,ISC40);//nastupna hrana
		
	sei(); // enable interrupts
}

void menu1(){
	UART_SendChar(27);//escape
	UART_SendString("[2J");//clear and home
	UART_SendChar(27);//escape
	UART_SendString("[0;32;40m");//barva pozadi a textu
	
	printf("-------------------------------------\n\r");
	printf("MENU: \n\r");
	printf("0 - Konec\n\r");
	printf("1 - Rozsvit LED1 \n\r");
	printf("2 - Zhasni LED1 \n\r");
	printf("3 - Jas LED0 \n\r");
	printf("4 - Start/stop Blikani LED2 \n\r");
	printf("5 - Teplota procesoru \n\r");
	printf("6 - Automaticky jas \n\r");
	printf("t - Teplota cidla I2C\n\r");
	printf("-------------------------------------\n\r");
	
	UART_SendChar(27);//escape
	UART_SendString("[0;37;40m");//reset
}
	
void menu3(){
	UART_SendChar(27);//escape
	UART_SendString("[2J");//clear and home
	UART_SendChar(27);//escape
	UART_SendString("[0;32;40m");//barva pozadi a textu
	
	printf("-------------------------------------\n\r");
	printf("Ovladani jasu: \n\r");
	printf("+ - Zvysit jas LED0\n\r");
	printf("- - Snizit jas LED0 \n\r");
	printf("q - Zpet \n\r");
	printf("-------------------------------------\n\r");
	
	UART_SendChar(27);//escape
	UART_SendString("[0;37;40m");//reset
}

void handle_menu1(){
	float teplota=0;
	switch (recv)  			{
		case '0':
		printf("\r\nEnd... \r\n");
		menu1(); 
		break;
		
		case '1':
		LED1ON;
		break;
		case '2':
		LED1OFF;
		break;
		case '3':
		menu_level=3;
		menu3();
		//body menu3();
		break;
		case '4':
		
		//LED2 = PORTB6 - OC1B -> Timer 1, Output compare A
		if (TCCR1B)
		{
			printf("\r\nKonec blikani \r\n");
			Timer1_Stop();
		}
		else
		{
			printf("\r\nStart blikani \r\n");
			Timer1_cmp_A_start(3000);//napriklad, podivejte se do funkce, je to normal mode + interrupt na Compare B
		}
		
		break;
		case '5':
		
		if (auto_reg==0) // pokud pouzivame automatickou regulaci jasu, tak to musime osetrit...
		{
			//ADC_Init(4,2);
			teplota = (1.13*ADC_readTemp())-272.8;
			printf("\n\rTeplota je: %f \n\r",teplota);
			ADC_stop();//zastavime - funkce vycisti vsechny konf. registry
		}
		else
		{
			//ADC_Init(4,2); //auto jas je, takze prenastavime adc
			teplota = (1.13*ADC_readTemp())-272.8;
			printf("\n\rTeplota je: %f \n\r",teplota);
			ADC_stop();//zastavime, vycistime
			ADC_Start_per(4,2,3,3);//a znovu spustime automaticky.
		}
		
			
		break;
		
		case '6':
		
		printf("\n\rAutomaticky jas: ");
		if (auto_reg==0)
		{
			printf(" zapnuto\r\n");
			auto_reg=1;
			//ADC_Init(4,2);
			ADC_Start_per(4,2,3,3);//4-prescale, 2-uref,3 - ADC3, 3 - Timer0COMPA
			Timer0_cmp_start(100);
		} 
		else
		{
			printf(" vypnuto\r\n");
			auto_reg=0;
			ADC_stop();
			Timer0_Stop(); //
		}
		
		
		break;
		case 't':
		printf("\n\Teplota cidla : ");
		{
			float teplota = AT30TSE758_read_temperature();
			printf("\n\rTeplota z cidla: %.2f celzia\n\r", teplota);
			break;
		}

		default:
		printf("\r\nUnknown command.. \r\n");
		//code - unknown command
		
	}
	char_to_process=0; //znak jsme zpracovali, tak nulujeme
}

	
void handle_menu3(){
	switch (recv)  			{
		
		case '+':
		printf("\r\nBrightness up.. \r\n");
		if (duty>STEP){
			duty=duty-STEP;
		}
		else {
			duty=0;
		}

		break;
		case '-':
		printf("\r\nBrightness down.. \r\n");
		if (duty<(100-STEP)){ //max duty  100
			duty=duty+STEP;
		}
		else {
			duty=100;
		}
		break;
		case 'q':
		menu_level=0;
		menu1();
		break;
		default:
		printf("\r\nUnknown command.. \r\n");
		//code - unknown command
		
	}
	char_to_process=0; //we have to zero this variable, because we already process it.
}



/************************************************************************/
/* MAIN                                                                 */
/************************************************************************/
int main(void)
{ 	board_init();
	_delay_ms(100);
	menu1();
    _delay_ms(100);
	
	int i=0;
	    while (1) 
    {
	    if (char_to_process==1){
		    switch (menu_level)
		    {
			    case 0 :
			    handle_menu1();
			    break;
			    case 3 :
			    handle_menu3();
			    break;
		    }
		    
	    }

	    _delay_ms(100);
		i++;
		//printf("Test x = %d \n\r", i);
		//printf("Klik = %d \n\r", klik);
    }
	
}
	


/************************************************************************/
/* INTERRUPTS                                                           */
/************************************************************************/

ISR(INT4_vect){
	//vzor obsluhy preruseni tlacitka, Button0 na hlavni desce
	//	"debounce"
	_delay_ms(200); // - zpozdeni - jen cekame az odezni zakmity
	klik++;
	LED3CHANGE;
	sbi(EIFR,INT4);// nakonci nastavime prizkak, ze doslo k preruseni na 1 - tim se vynuluje (ano, je to zvlastni, ale je to tak) 
}

ISR(USART1_RX_vect)
{
	recv = UART_GetChar(); //
	UART_SendChar(recv);//echo
	char_to_process=1; //state machine - osluhu prikazu nebudeme delat v preruseni.
}


ISR (TIMER0_COMPA_vect)
{
	//pro jistotu;}
ISR (TIMER1_COMPA_vect)
{
	//LED3CHANGE;}ISR (TIMER1_COMPB_vect)
{
	//Musíme vynulovat, proc? Timer1 ma CTC mod svazany s Compare A... my použijeme lehce jinak...	//TCNT1=0;}ISR (TIMER2_OVF_vect)
{
	OCR2A = (255*duty)/100; //OCRxx se ve FastPWM aktualizuje v TOP...
}ISR(ADC_vect)
{
			
	if (auto_reg==1)
	{
		volatile uint16_t tmp=0;
		tmp=ADC;
		duty=100-((float)tmp/1024*100);
	}
	
}