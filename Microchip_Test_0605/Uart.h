/*
 * Uart.h
 *
 * Created: 2025-06-12 오전 9:33:31
 *  Author: Kujabeen
 */ 
//#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/**********통신관련*************/

#define STX					0xF2
#define ETX					0xF3

#define UART0_MAXBUF		200
#define UART1_MAXBUF		250

void USART0_init(void);
void USART1_init(void);



extern unsigned char RxBuf1[UART1_MAXBUF], Data1, RxNum1;
//extern int Rx_data_trans;