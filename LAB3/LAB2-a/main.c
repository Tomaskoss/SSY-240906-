/*
 * LAB2-a.c
 *
 * Created: 15.02.2023 12:18:20
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

/************************************************************************/
/* DEFINES                                                              */
/************************************************************************/

// F_CPU definovano primo v projektu!!!Defined in project properties

/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/

FILE uart_str = FDEV_SETUP_STREAM(printCHAR, NULL, _FDEV_SETUP_RW);//soubor pro stdout
volatile uint8_t pwm_duty_cycle = 128; // Prednastavený stredný jas

volatile uint8_t is_pwm_updating = 0; // Stavová premenná pre ovládanie PWM
volatile int pruchod=0;  //
volatile uint8_t klik=0; //
volatile char recv;
volatile uint8_t state=0;
volatile uint8_t menu = 1;  // Premenná na ur?enie aktuálneho menu
char text[]="label"; //= 'l''a''b''e''l''/0'

/************************************************************************/
/* PROTOTYPES                                                           */
/************************************************************************/

void board_init();
void menu1();
void Timer1_cmp_start(int porovnani);
void prvniUkoly();

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
void board_init(){
	cli(); //disable interrupts - defaultne je sice vyple, ale pro jistotu
	
	UART_init(38400); //baudrate 38400b/s
	
	UCSR1B |= (1 << RXCIE1);// UART receive interrupt enable
	//sbi(UCSR1B,RXCIE1);
	stdout = &uart_str; //standartní výstup/std output
	
	//LED0 vystup/output
	sbi(DDRB,4);
	sbi(PORTB,4); //turn the LED0 down

	//LED1
	sbiX(DDRB,5);
	sbi(PORTB,5);
	
	//Tlacitka vstup/buttons as input
	cbi(DDRE,4);
	//Tlacitka pull-up
	sbi(PORTE,4);
	
	sbi(EIMSK,4); //povolit preruseni INT5 - tlacitko button0 /INT5 enable
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
	printf("3 - Menu 2 \n\r");
	printf("4 - Vypis abecedu \n\r");
	printf("5 - Generovanie casovace 2Hz \n\r");
	printf("-------------------------------------\n\r");
	
	UART_SendChar(27);//escape
	UART_SendString("[0;37;40m");//reset
}


void menu2(){
		printf("-------------------------------------\n\r");
		printf("MENU: \n\r");
		printf("0  zpet do menu1  \n\r");
		printf("+  Pridanie Jasu \n\r");
		printf("-  Znizenie Jasu \n\r");
		printf("-------------------------------------\n\r");
}

void prvniUkoly(){
	DDRB=0b01110000;
	PORTB=0b01110000;
	_delay_ms(500);
	PORTB=0;
	_delay_ms(500);
	
	PORTB=112;
	_delay_ms(500);
	PORTB=0;
	_delay_ms(500);
	
	PORTB=0x70;
	_delay_ms(500);
	PORTB=0;
	_delay_ms(500);
	
	PORTB=(7<<4);
	_delay_ms(500);
	PORTB=0;
	_delay_ms(500);
	
	DDRB=0;
	PORTB=0;
	
}

/************************************************************************/
/* MAIN                                                                 */
/************************************************************************/
int main(void)
{ 	
	prvniUkoly(); // jen blikani LED, 
	
	board_init();
	_delay_ms(100);
	menu1();
	_delay_ms(100);
	
	int i=0;
	 while (1) {
		 if (state == 1) {
			 if (menu == 2) {  // Ak sme v menu2
				 switch (recv) {
					 case '0':
					 printf("\r\nKoncim program... \r\n");
					 menu1();  // Prejdeme spä? do menu1
					 menu = 1;  // Nastavíme menu na 1
					 break;

					 case '+':  // Pridanie jasu
					change_brightness(10);  // Zvyšujeme jas
					break;

				case '-':  // Zníženie jasu
					change_brightness(-10);  // Znižujeme jas
					break;

					 case '3':  // Prechod do menu1
					 printf("\r\nPrechádzam do menu 1...\r\n");
					 menu1();
					 menu = 1;  // Nastavíme menu na 1
					 break;

					 default:
					 printf("\r\nNeznamy prikaz.. \r\n");
					 break;
				 }
				 } else if (menu == 1) {  // Ak sme v menu1
				 switch (recv) {
					 case '0':
					 printf("\r\nKoncim program... \r\n");
					 menu1();  // Zobrazíme menu1
					 break;

					 case '1':
					 LED1ON;
					 printf("\r\n");
					 break;

					 case '2':
					 printf("\r\n");
					 LED1OFF;
					 break;

					 case '3':  // Prechod do menu2
					 printf("\r\nPrechadzam do menu 2...\r\n");
					 menu2();
					 menu = 2;  // Nastavíme menu na 2
					 break;

					 case '4':
					 printf("\r\n");
					 for (int i = 65; i < 90; i++) {
						 UART_SendChar(i);
					 }
					 printf("\n\r");
					 break;

					 case '5':
					 printf("\r\nSpustenie TIMER1 na 2 Hz\r\n");
					 Timer1_cmp_start(7812);  // Spustí generovanie 2 Hz na OC1A (LED1)
					 break;

					 default:
					 printf("\r\nNeznamy prikaz.. \r\n");
					 break;
				 }
			 }
			 state = 0;
		 }
		 _delay_ms(100);
	 }
 }

/************************************************************************/
/* INTERRUPTS                                                           */
/************************************************************************/

ISR(INT4_vect){
	//	"debounce", hodne spatny debounce....
	cbi(EIMSK,INT4);//zakazeme preruseni INT4
	
	_delay_ms(200); // - zpozdeni - jen cekame az odezni zakmity
	klik++;
	LED0CHANGE;
	sbi(EIFR,INTF4);// nakonci nastavime prizkak, ze doslo k preruseni na 1 - tim se vynuluje (ano, je to zvlastni, ale je to tak)
	sbi(EIMSK,INT4);//Povolime preruseni INT4
}

ISR(USART1_RX_vect)
{
	recv = UART_GetChar(); // zbytecné
	//recv = UDR1;//nejspíš lepší
	UART_SendChar(recv);//echo, pro jistotu...Jinak nebudeme videt, co piseme
	state=1; //state machine
}

void Timer1_cmp_start(int porovnani){
	cli (); // zakazat preruseni
	TCCR1A = 0; // vycistit kontrolni registry
	TCCR1B = 0;
	TIMSK1 = 0;
	// nastavit hodnotu pro porovnani
	OCR1A = porovnani;
	// ctc mod
	TCCR1B |= (1 << WGM12);
	//1024 preddelicka :
	TCCR1B |= (1 << CS12) | (1 << CS10); // Nastavenie prescalera 1024
	// povolit preruseni, pokud budete potrebovat
	TIMSK1 |= (1 << OCIE1A);
	//vystup na pinc OC1A, toogle
	TCCR1A |= (1 << COM1A0);
	sei(); //povolit globalni preruseni
}

void Timer2_fastpwm_start(uint8_t strida){
	cli ();
	TCCR2A = 0;
	TCCR2B = 0;
	TIMSK2 = 0;
	
 
	
	
	//nastavit hodnotu pro PWM
	OCR2A = strida;
	//fastpwm mod:
	TCCR2A |= (1 << WGM21);
	TCCR2A |= (1 << WGM20);
	// 1024 preddelicka>
	TCCR2B |= 5;
	  // Nastavi? prescaler 1024 pre TIMER2
	//TCCR2B |= (1 << CS22) | (1 << CS20);  // Prescaler = 1024
	//povolit preruseni, 
	TIMSK2 |= (1 << TOIE2);
	//vystup na pin OC2A
	TCCR2A |= (1 << COM2A1);
	sei(); // povolit globalni preruseni 
}


ISR (TIMER1_COMPA_vect)
{
	LED3CHANGE; //nap?., v pripade rezimu s porovnanim a kratkymi casy, staci...
	
}
void change_brightness(int change) {
    pwm_duty_cycle += change; // Zmeníme hodnotu striedy
    if (pwm_duty_cycle > 255) {
        pwm_duty_cycle = 255; // Ochrana pred prekro?ením maximálnej hodnoty
    } else if (pwm_duty_cycle < 0) {
        pwm_duty_cycle = 0; // Ochrana pred prekro?ením minimálnej hodnoty
    }
    // Nastavíme novú striedu na Timer2
    Timer2_fastpwm_start(pwm_duty_cycle);
}