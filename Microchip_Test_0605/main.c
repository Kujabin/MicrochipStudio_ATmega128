/*
 * Microchip_Test_0605.c
 *
 * Created: 2025-06-05 오후 1:20:01
 * Author : Kujabeen
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 14745600UL
#include <util/delay.h>
#include <stdio.h>	//
#include <stdarg.h>	//
#include <stdint.h>	// 
#include <string.h>
#include <ctype.h>  // 대소문자 무시하고 비교하려면 필요
#include "Uart.h"
#include "main.h"



volatile uint8_t led_flag = 0;

//---------------------------------------------------------------------------
#define TIMER0_MAX_COUNT (1UL << (sizeof(OCR0) * 8))     // 256
#define TIMER1_MAX_COUNT (1UL << (sizeof(OCR1A) * 8))    // 65536

#define TIMER0_MAX_SEC ((TIMER0_MAX_COUNT * 1024.0) / F_CPU)
#define TIMER1_MAX_SEC ((TIMER1_MAX_COUNT * 1024.0) / F_CPU)
//---------------------------------------------------------------------------

#define STATIC_CHECK_TIMER0(sec) \
_Static_assert((sec) <= TIMER0_MAX_SEC, \
"***Timer0 Error: Time exceeds ~17.7ms***")

#define STATIC_CHECK_TIMER1(sec) \
_Static_assert((sec) <= TIMER1_MAX_SEC, \
"***Timer1 Error: Time exceeds ~4.44s***")

////////////////////////////////////////////////////////unsigned char RxBuf1[UART1_MAXBUF], Data1, RxNum1;
int Timer_485 = 0;

void Timer0_init_1ms(void) {
	TCCR0 = (1 << WGM01);              // CTC 모드
	OCR0 = 230;                        // 1ms마다 비교일치 (계산식 참고 아래)
	TIMSK |= (1 << OCIE0);            // 비교일치 인터럽트 허용
	TCCR0 |= (1 << CS01) | (1 << CS00); // 분주비 64
}

// Timer1: 1초 인터럽트
void Timer1_init_1s(void) {
	TCCR1B |= (1 << WGM12);           // CTC 모드
	OCR1A = 14399;                    // 1초마다 비교일치 (계산식 아래 참고)
	TIMSK |= (1 << OCIE1A);           // 비교일치 인터럽트 허용
	TCCR1B |= (1 << CS12) | (1 << CS10); // 분주비 1024
	
}

/*	 /////////////////////////////////////////////////////////////////////////////////////////////////uart.c
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
*/

//-------------------------------------------------------------port, pin 개별 제어
volatile uint8_t* get_ddr(uint8_t pin) {
	if (pin <= PA7) return &DDRA;
	else if (pin <= PB7) return &DDRB;
	else if (pin <= PC7) return &DDRC;
	else if (pin <= PD7) return &DDRD;
	else if (pin <= PE7) return &DDRE;
	else if (pin <= PF7) return &DDRF;
	else return &DDRG;
}

volatile uint8_t* get_port(uint8_t pin) {
	if (pin <= PA7) return &PORTA;
	else if (pin <= PB7) return &PORTB;
	else if (pin <= PC7) return &PORTC;
	else if (pin <= PD7) return &PORTD;
	else if (pin <= PE7) return &PORTE;
	else if (pin <= PF7) return &PORTF;
	else return &PORTG;
}

volatile uint8_t* get_pin(uint8_t pin) {
	if (pin <= PA7) return &PINA;
	else if (pin <= PB7) return &PINB;
	else if (pin <= PC7) return &PINC;
	else if (pin <= PD7) return &PIND;
	else if (pin <= PE7) return &PINE;
	else if (pin <= PF7) return &PINF;
	else return &PING;
}

uint8_t get_bit(uint8_t pin) {
	return pin % 8;
}

void pinMode(PinName pin, uint8_t mode) {			//입출력 제어 1: 출력, 0: 입력
	volatile uint8_t* ddr;
	uint8_t bit;

	if (pin >= PIN_PA0 && pin <= PIN_PA7) {
		ddr = &DDRA;
		bit = pin - PIN_PA0;
		} else if (pin >= PIN_PB0 && pin <= PIN_PB7) {
		ddr = &DDRB;
		bit = pin - PIN_PB0;
		} else if (pin >= PIN_PC0 && pin <= PIN_PC7) {
		ddr = &DDRC;
		bit = pin - PIN_PC0;
		} else if (pin >= PIN_PD0 && pin <= PIN_PD7) {
		ddr = &DDRD;
		bit = pin - PIN_PD0;
	} else return;

	if (mode)
	*ddr |= (1 << bit);
	else
	*ddr &= ~(1 << bit);
}

void digitalWrite(PinName pin, uint8_t value) {		//Pin High, Low 제어
	volatile uint8_t* port;
	uint8_t bit;

	if (pin >= PIN_PA0 && pin <= PIN_PA7) {
		port = &PORTA;
		bit = pin - PIN_PA0;
		} else if (pin >= PIN_PB0 && pin <= PIN_PB7) {
		port = &PORTB;
		bit = pin - PIN_PB0;
		} else if (pin >= PIN_PC0 && pin <= PIN_PC7) {
		port = &PORTC;
		bit = pin - PIN_PC0;
		} else if (pin >= PIN_PD0 && pin <= PIN_PD7) {
		port = &PORTD;
		bit = pin - PIN_PD0;
	} else return;

	if (value)
	*port |= (1 << bit);
	else
	*port &= ~(1 << bit);
}
//------------------------------------------------------------------------
// RS-485 송신 모드 설정 함수
void RS485_tx_mode(void) {
	RS485_EN(1);
	_delay_us(1000);

}

// RS-485 수신 모드 설정 함수
void RS485_rx_mode(void) {
	_delay_us(2000);
	RS485_EN(0);

}
//-------------------------------------------------------------------------
void Tx0Data(char data)		// RS232
{
	while(!(UCSR0A & 0x20));
	UDR0=data;
}

void Tx1Data(char data)		// RS485
{
	while(!(UCSR1A & 0x20));
	UDR1=data;
}

void Tx1String(const char* str) {
	while (*str) {
		if (*str == '\n') {
			Tx1Data(0x0A);  // Line Feed
			} else if (*str == '\r') {
			Tx1Data(0x0D);  // Carriage Return
			} else {
			Tx1Data(*str);  // 일반 문자
		}
		str++;
	}
}

// 기본 printf와 같이 사용 가능한 함수
void Tx1Printf(const char* format, ...) {
	char buffer[128];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);  // 가변 인자 처리
	va_end(args);

	for (char* p = buffer; *p; p++) {
		if (*p == '\n') {
			Tx1Data('\r');  // 줄바꿈을 위해 CR 추가
		}
		Tx1Data(*p);
	}
}

void Tx1Println(const char* format, ...) {
	char buffer[128];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

	for (char* p = buffer; *p; p++) {
		if (*p == '\n') {
			Tx1Data('\r');
		}
		Tx1Data(*p);
	}
	// 줄바꿈 추가
	Tx1Data('\r');
	Tx1Data('\n');
}

// 1. 485통신으로 버전 송신
// 2. 버전을 선언 ( define or int char )
// 3. 설정한 버전의 자리수만큼 485통신으로 데이터를 송신 ( 핵심 기능 )
// 4. 1바이트 데이터만 송신가능하니 버전을 자리수만큼 잘라서 아스키로 보내자 ( 기능의 문제를 확인하고, 조치 )
// 5. 버전을 자리수만큼 잘라야하니 연산자를 사용
void Ver_check(void)
{
	Tx1Println("Prog Ver: %06ld", Prog_Ver);
}

/*	 /////////////////////////////////////////////////////////////////////////////////////////////////uart.c
// USART0 수신 인터럽트
ISR(USART0_RX_vect) {
	//char Data0 = UDR0;
	
// 	RxBuf0[RxNum0++] = Data0;
// 	if (RxNum0 >= UART0_MAXBUF) RxNum0 = 0;
// 	
// 	if(RxBuf0[RxNum0-2]==0x0D && RxBuf0[RxNum0-1]==0x0A)
// 	{
// 		if(RxNum0 != 10)
// 		{
// 			RxNum0 = 0;
// 		}
// 	}
	
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
*/

ISR(TIMER1_COMPA_INTERRUPT) {		//1s timer interrupt
	Timer_485++;
	led_flag ^= 1;  // 0 → 1 → 0 → 1 토글
	//Tx1Data(led_flag+0x30);	//test
}




int main(void)
{
	#ifdef change_Rx_Tx_mode
	int i = 0;
	char mode = 0;
	#endif
	
	pinMode(PIN_PD4, 1);	//led
	
 	DDRB = 0xFF;
	
	Timer1_init_1s();
	USART0_init();
	USART1_init();
	sei();  // 전역 인터럽트 활성화
	_delay_ms(1000);	//업데이트 및 보드 전원 재 인가 시, 3번씩 main문 동작
	RS485_tx_mode();
	
	Ver_check();
	
	digitalWrite(PIN_PD4, 1);	//led off
	
	Timer_485 = 0;
	Tx1Println("Select the mode you want");
	Tx1Println("0x31:Tx / 0x32:Rx");
	Tx1Println("ex) [F2] [1] [2] [F3]");
	Tx1Println("=====================");
	while (1)
	{
		#ifdef change_Rx_Tx_mode
		RS485_rx_mode();
		if(RxBuf1[0] == STX && RxBuf1[RxNum1-1] == ETX)
		{
			if(RxBuf1[1] == '1')
			{
				if(mode != Tx)
				{
					RS485_tx_mode();
					Tx1Println("Select Tx mode");
					mode = Tx;
				}
				RxNum1 = 0;
			}
			else if(RxBuf1[1] == '2')
			{
				if(mode != Rx)
				{
					RS485_tx_mode();
					Tx1Println("Select Rx mode");
				}
				i = RxBuf1[2];
				RS485_tx_mode();
				
				if (i == 0xF3) {
					Tx1Println("illegal data");
				}
				else
				{
					Tx1Println("RxBuf1[2] is %X",i);
				}
				RS485_rx_mode();
				mode = Rx;
				RxNum1 = 0;
			}
		}
		#endif

		#ifdef Tx_Rx_repeat
		if(Timer_485 == 5)
		{
			if(led_flag)
			{
				RS485_tx_mode();
	
				Tx1String("\r\n Tx Mode");
			}
			else
			{
				Tx1String("\r\n Rx Mode: ");
				RS485_rx_mode();
				if(RxBuf1[0] == STX && RxBuf1[RxNum1-1] == ETX)
				{
					if(RxBuf1[1] == '1')
					{
						Tx1Data(RxBuf1[1]);
						RxNum1 = 0;
					}
					if(RxBuf1[1] == '2')
					{
						Tx1Data(RxBuf1[1]);
						RxNum1 = 0;
					}
					
				}
				
			}
			Timer_485 = 0;
		}
		#endif

		#ifdef led_toggle_test
		if(led_flag == 0)
		{
			digitalWrite(PIN_PD4, 0);  //PORTD = 0xEF;	//led on
		}
		else if(led_flag == 1)
		{
			digitalWrite(PIN_PD4, 1);  //PORTD = 0xFF;	//led off
		}
		#endif
	}	//end while
}	//end main




