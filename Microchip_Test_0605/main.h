/*
 * main.h
 *
 * Created: 2025-06-05 오후 2:32:30
 *  Author: Kujabeen
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define change_Rx_Tx_mode
//#define Tx_Rx_repeat
#define led_toggle_test

#define RS485_EN(n) ((n) ? (PORTB |= (1 << PORTB4)) : (PORTB &= ~(1 << PORTB4)))	//1이면 Tx(송신), 0이면 Rx(수신)


typedef enum {
	PIN_PA0, PIN_PA1, PIN_PA2, PIN_PA3, PIN_PA4, PIN_PA5, PIN_PA6, PIN_PA7,
	PIN_PB0, PIN_PB1, PIN_PB2, PIN_PB3, PIN_PB4, PIN_PB5, PIN_PB6, PIN_PB7,
	PIN_PC0, PIN_PC1, PIN_PC2, PIN_PC3, PIN_PC4, PIN_PC5, PIN_PC6, PIN_PC7,
	PIN_PD0, PIN_PD1, PIN_PD2, PIN_PD3, PIN_PD4, PIN_PD5, PIN_PD6, PIN_PD7,
	PIN_PE0, PIN_PE1, PIN_PE2, PIN_PE3, PIN_PE4, PIN_PE5, PIN_PE6, PIN_PE7,
	PIN_PF0, PIN_PF1, PIN_PF2, PIN_PF3, PIN_PF4, PIN_PF5, PIN_PF6, PIN_PF7,
	PIN_PG0, PIN_PG1, PIN_PG2, PIN_PG3, PIN_PG4, PIN_PG5, PIN_PG6, PIN_PG7,
} PinName;

// RS-485 제어 핀 정의
#define RS485_EN_PIN PIN_PB4

#define Prog_Ver	250624

#define	TIMER1_COMPA_INTERRUPT	TIMER1_COMPA_vect		//timer1 interrupt vector name change

/**********통신관련*************/

// #define STX					0xF2
// #define ETX					0xF3
// 
//  #define UART0_MAXBUF		200
//  #define UART1_MAXBUF		250


#define Tx				1
#define Rx				2

#define LED_RED	PD4