/*
 * Project_Bilka.c
 *
 * Created: 3/31/2025 11:59:26
 * Author : Student
 */  

#include <avr/io.h>
/*- Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "config.h"
#include "hal.h"
#include "phy.h"
#include "sys.h"
#include "nwk.h"
#include "sysTimer.h"
#include "halBoard.h"
#include "halUart.h"
#include "main.h"
#include "makra.h"
#include "uart/uart.h"


/*- Definitions ------------------------------------------------------------*/
#ifdef NWK_ENABLE_SECURITY
#define APP_BUFFER_SIZE     (NWK_MAX_PAYLOAD_SIZE - NWK_SECURITY_MIC_SIZE)
#else
#define APP_BUFFER_SIZE     NWK_MAX_PAYLOAD_SIZE
#endif

/*- Types ------------------------------------------------------------------*/
typedef enum AppState_t
{
	APP_STATE_INITIAL,
	APP_STATE_SEND_CHAR,
	APP_STATE_IDLE,
} AppState_t;

/*- Prototypes -------------------------------------------------------------*/
static void appSendData(void);
void board_init();
/*- Variables --------------------------------------------------------------*/
static AppState_t appState = APP_STATE_INITIAL;
static SYS_Timer_t appTimer;
static NWK_DataReq_t appDataReq;
static bool appDataReqBusy = false;
static uint8_t appDataReqBuffer[APP_BUFFER_SIZE];
static uint8_t appUartBuffer[APP_BUFFER_SIZE];
static uint8_t appUartBufferPtr = 0;

/*- Implementations --------------------------------------------------------*/

FILE uart_str = FDEV_SETUP_STREAM(printCHAR, NULL, _FDEV_SETUP_RW);//soubor pro stdout

char text[]="label"; //= 'l''a''b''e''l''/0'


void board_init(){
	
	
	UART_init(38400); //baudrate 38400b/s
	
	UCSR1B |= (1 << RXCIE1);// UART receive interrupt enable
	//sbi(UCSR1B,RXCIE1);
	stdout = &uart_str; //standartní výstup/std output
	
}

// Inicializácia ADC
void adc_init() {
	// Nastavenie referen?ného napätia na 1.5V (interný referen?ný zdroj)
	ADMUX = (1 << REFS1);  // REFS1 = 1 (VREF = 1.5V), REFS0 = 0 (VREF = 1.5V)
	
	// Povolenie ADC a nastavenie preddelova?a na 128
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

// cítanie hodnoty z ADC
uint16_t adc_read(uint8_t channel) {
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);  // Výber kanálu
	ADCSRA |= (1 << ADSC);  // Spustenie konverzie
	while (ADCSRA & (1 << ADSC));  // cakaj na skoncenie konverzie
	return ADC;
} 

/*************************************************************************//**
*****************************************************************************/
static void appDataConf(NWK_DataReq_t *req)
{
	appDataReqBusy = false;
	(void)req;
}

/*************************************************************************//**
*****************************************************************************/
static void appSendData(void)
{
if (appDataReqBusy || 0 == appUartBufferPtr)
return;

memcpy(appDataReqBuffer, appUartBuffer, appUartBufferPtr);

appDataReq.dstAddr = 1-APP_ADDR;
appDataReq.dstEndpoint = APP_ENDPOINT;
appDataReq.srcEndpoint = APP_ENDPOINT;
appDataReq.options = NWK_OPT_ENABLE_SECURITY;
appDataReq.data = appDataReqBuffer;
appDataReq.size = appUartBufferPtr;
appDataReq.confirm = appDataConf;
NWK_DataReq(&appDataReq);

appUartBufferPtr = 0;
appDataReqBusy = true;
}

/*************************************************************************//**
*****************************************************************************/
void HAL_UartBytesReceived(uint16_t bytes)
{
	
	for (uint16_t i = 0; i < bytes; i++)
	{
	uint8_t byte = HAL_UartReadByte();

	if (appUartBufferPtr == sizeof(appUartBuffer))
	appSendData();

	if (appUartBufferPtr < sizeof(appUartBuffer))
	appUartBuffer[appUartBufferPtr++] = byte;
	}

	SYS_TimerStop(&appTimer);
	SYS_TimerStart(&appTimer);

}


void sendFSRValue() {
    if (appDataReqBusy || 0 == appUartBufferPtr)
        return;	

    appDataReq.dstAddr = 1-APP_ADDR;  // adresa prijímate?a (uprav pod?a siete)
    appDataReq.dstEndpoint = APP_ENDPOINT;
    appDataReq.srcEndpoint = APP_ENDPOINT;
    appDataReq.options = NWK_OPT_ENABLE_SECURITY;
    appDataReq.data = appDataReqBuffer;
    appDataReq.confirm = appDataConf;

    NWK_DataReq(&appDataReq);
	appUartBufferPtr=0;
    appDataReqBusy = true;


}

/*************************************************************************//**
*****************************************************************************/
static void appTimerHandler(SYS_Timer_t *timer)
{
appSendData();
(void)timer;
}

/*************************************************************************//**
*****************************************************************************/
static bool appDataInd(NWK_DataInd_t *ind)
{
static NWK_DataReq_t response;
 

    if (appDataReqBusy)  // Pokud je odesílání obsazené, zprávu ignorujeme
        return false;

    	printf("Prijata ADC Value:");
	for(uint8_t i=0; i< ind->size; i++){
		printf("[%c]",ind->data[i]);
	}
	printf("\n\r");

return true;
}

/*************************************************************************//**
*****************************************************************************/
static void appInit(void)
{
NWK_SetAddr(APP_ADDR);
NWK_SetPanId(APP_PANID);
PHY_SetChannel(APP_CHANNEL);
#ifdef PHY_AT86RF212
PHY_SetBand(APP_BAND);
PHY_SetModulation(APP_MODULATION);
#endif
PHY_SetRxState(true);

NWK_OpenEndpoint(APP_ENDPOINT, appDataInd);

HAL_BoardInit();

appTimer.interval = APP_FLUSH_TIMER_INTERVAL;
appTimer.mode = SYS_TIMER_INTERVAL_MODE;
appTimer.handler = appTimerHandler;
}

/*************************************************************************//**
*****************************************************************************/
static void APP_TaskHandler(void)
{
 static char buffer[16]; // buffer na uloženie textu s ADC hodnotou
 static uint16_t adc_value; // ADC hodnota

 switch (appState)
 {
	 case APP_STATE_INITIAL:
	 {
		 appInit();
		 appState = APP_STATE_IDLE;
	 } break;

	 case APP_STATE_SEND_CHAR:
	 {
		adc_value = adc_read(0); // na?ítaj ADC hodnotu z kanála 0 (PF0)

		 // preved hodnotu na string
		snprintf(buffer, sizeof(buffer), "%u", adc_value); // napríklad "1234"
		   // Skontroluj, ?i je miesto obsadené alebo nie
	
		 // skopíruj string do appUartBuffer
		 appUartBufferPtr = strlen(buffer);
		 memcpy(appUartBuffer, buffer, appUartBufferPtr);

		 appSendData(); // odošli dáta cez sie?

		 printf("Odoslana ADC Value: %u\n\r", adc_value); // vypíš do UART
			
		// Skontroluj, ?i je miesto obsadené alebo nie
		if (adc_value > 100) {
			printf("Miesto je obsadene!\n\r");
		} else {
			printf("Miesto nie je obsadene.\n\r");
		}

		 appState = APP_STATE_IDLE;
	 } break;

	 case APP_STATE_IDLE:
	 break;

	 default:
	 break;
 }
}


/*************************************************************************//**
*****************************************************************************/
int main(void)
{
SYS_Init();

// Inicializácia UART
board_init();
adc_init();   // Inicializuj ADC

char buffer[32];
uint16_t adc_value;


HAL_UartInit(38400);
HAL_UartWriteByte('a');

UART_SendChar(27);//escape
UART_SendString("[2J");//clear and home
UART_SendChar(27);//escape
UART_SendString("[0;32;40m");//barva pozadi a textu

// Vytlac uvítaciu správu
printf("-------------------------------------\n\r");
printf("Vitaj v programe pre snimanie obsadenia pomocou FSR senzora!\n\r");
while (1)
{
	stdout=&uart_str;
	
     SYS_TaskHandler();
     HAL_UartTaskHandler();
     APP_TaskHandler();
	 if(!appDataReqBusy && appState == APP_STATE_IDLE){
		 appState = APP_STATE_SEND_CHAR;
		 _delay_ms(500);
	 }
     }
	
	}

