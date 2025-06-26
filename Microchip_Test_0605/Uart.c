/*
 * Uart.c
 *
 * Created: 2025-06-12 오전 9:20:37
 *  Author: Kujabeen
 */ 
#define F_CPU 14745600UL
#include "uart.h"

#include <avr/io.h>
#include <util/delay.h>

unsigned char RxBuf1[UART1_MAXBUF], Data1, RxNum1;
int Rx_data_trans = 0;


void USART0_init(void) {
	UBRR0H = 0;
	UBRR0L = 191; //191 = 0xBF // 9600bps @ 14.7456MHz with U2X
	UCSR0A = (1 << U2X0); // 2배속 모드
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // 송수신 + 인터럽트
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit
}

void USART1_init(void) {
	UBRR1H = 0;
	UBRR1L = 0xBF;	//191 = 0xBF //14745600Hz with U2X

	UCSR1A=0x02;	//2배속 비동기 통신 모드	//115200이상의
	UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);  // 수신, 송신, 수신 인터럽트 허용
	UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);  // 8-bit 데이터, 비패리티, 1 스탑비트
}

// USART0 수신 인터럽트
ISR(USART0_RX_vect) {
	//char Data0 = UDR0;
	/*
	RxBuf0[RxNum0++] = Data0;
	if (RxNum0 >= UART0_MAXBUF) RxNum0 = 0;
	
	if(RxBuf0[RxNum0-2]==0x0D && RxBuf0[RxNum0-1]==0x0A)
	{
		if(RxNum0 != 10)
		{
			RxNum0 = 0;
		}
	}
	*/
	//Tx0Data(data); // 수신된 걸 다시 전송 (에코)
}

// USART1 수신 인터럽트
ISR(USART1_RX_vect) {
	//int Rx_data = 5;
	int Rx_data = RxBuf1[1];
	Rx_data_trans = Rx_data;
	char Data1 = UDR1;
	if(Data1 == STX) RxNum1 = 0;
	RxBuf1[RxNum1++] = Data1;
	if (RxNum1 >= UART1_MAXBUF) RxNum1 = 0;
	
	PORTD ^= ( 1 << PORTD4 );

	//Tx1Data(Data1); // 수신된 걸 다시 전송 (에코)
}

void USART1_transmit(uint8_t data) {
	while (!(UCSR1A & (1 << UDRE1)));  // 송신 버퍼 대기
	UDR1 = data;
}