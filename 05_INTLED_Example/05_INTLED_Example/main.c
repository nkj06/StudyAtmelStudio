/*
 * INTLED_Example.c
 *
 * Created: 2020-06-15 오후 2:13:45
 * Author : PKNU
 * 설명 : 현재 수행중인 일을 잠시 중단하고 급한 일을 처리한 후, 원래의 일을 다시 이어서 수행
 */ 
/*
// 인터럽트로 LED 점멸
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned char Time_STOP = 0;

int main(void)
{
    unsigned char LED_Data = 0x01;
	
	DDRC = 0x0F; // LED
	DDRA = 0x00; // 스위치
	EICRB = 0x03;
	EIMSK = 0x10;
	EIFR = 0x10;
	sei();
	
    while (1) 
    {
		PORTC = LED_Data;
		if(Time_STOP == 0)
		{
			if(LED_Data == 0x08) LED_Data = 0x01;
			else LED_Data <<= 1;
		}
		_delay_ms(100);
    }
}

SIGNAL(INT4_vect)
{
	cli();
	if(Time_STOP == 0)
	{
		Time_STOP = 1;
	}
	else
	{
		Time_STOP = 0;
	}
	sei();
}
*/
/*
// 인터럽트로 LED의 좌우 이동 방향 변경하기
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned char Shift_flag = 1;

int main(void)
{
	unsigned char LED_Data = 0x01;
	
	DDRC = 0x0F; // LED
	DDRA = 0x00; // 스위치
	
	// 인터럽트 5를 하강엣지, 7을 상승엣지에서 동작하도록 설정
	// 하강엣지는 1에서 0으로 변경되는 시점, 상승엣지는 0에서 1로 변경되는 시점
	EICRB = 0xC8;
	
	EIMSK = 0xA0;  // 인터럽트 5, 7을 허용
	EIFR = 0xA0;  // 인터럽트 5, 7 플래그를 클리어
	
	sei(); // 전체 인터럽트를 허용 
	
	while (1)
	{
		PORTC = LED_Data; // 포트 c로 변수 LED_Data에 있는 데이터를 출력
		if(Shift_flag == 1) // LED 0~3으로 이동
		{
			if(LED_Data == 0x08) LED_Data = 0x01;
			else LED_Data <<= 1;
		}
		else if(Shift_flag == 2) // LED 3~0으로 이동
		{
			if(LED_Data == 0x01) LED_Data = 0x08;
			else LED_Data >>= 1;
		}
		_delay_ms(100);
	}
}

SIGNAL(INT5_vect) // 인터럽트 서비스 루틴
{
	cli(); // 전체 인터럽트를 금지
	Shift_flag = 1; 
	sei(); // 전체 인터럽트를 허용
}

SIGNAL(INT7_vect)
{
	cli();
	Shift_flag = 2;
	sei();
}
*/
/*
// 인터럽트로 FND 점멸
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned char Time_STOP = 0;

int main(void)
{
	unsigned char FND_DATA_TBL [] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7C,0x07,0x7F,
									0x67,0x77,0x7C,0x39,0x5E,0x79,0x71,0x08,0x80}; // 0~9, A~F
	
	unsigned char cnt = 0;
	DDRA = 0xFF; // FND
	DDRE = 0x00; // 스위치
	
	// 인터럽트 5를 상승엣지
	// 하강엣지는 1에서 0으로 변경되는 시점, 상승엣지는 0에서 1로 변경되는 시점
	EICRB = 0x0C;
	
	EIMSK = 0x20;  // 인터럽트 5를 허용
	EIFR = 0x20;  // 인터럽트 5 플래그를 클리어
	
	sei(); // 전체 인터럽트를 허용
	
	while (1)
	{
		PORTA = FND_DATA_TBL[cnt];
		if(Time_STOP == 0)
		{
			cnt++;
			if(cnt>17) cnt = 0;
		}
		_delay_ms(200);
	}
}

SIGNAL(INT5_vect) // 인터럽트 서비스 루틴
{
	cli(); // 전체 인터럽트를 금지
	if(Time_STOP == 0)
	{
		Time_STOP = 1;
	}
	else
	{
		Time_STOP = 0;
	}
	sei();
}
*/

/*
// 인터럽트로 FND LED 값 변경하기
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned char Time_STOP = 0;
volatile unsigned char cnt = 0; // FND Table 카운터 변수

int main(void)
{
	unsigned char FND_DATA_TBL [] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7C,0x07,0x7F,
									0x67,0x77,0x7C,0x39,0x5E,0x79,0x71,0x08,0x80}; // 0~9, A~F

	DDRA = 0xFF;
	DDRE = 0x00;
	
	// 인터럽트 4를 하강엣지, 6을 상승엣지
	// 하강엣지는 1에서 0으로 변경되는 시점, 상승엣지는 0에서 1로 변경되는 시점
	EICRB = 0x32;
	
	EIMSK = 0x50;  // 인터럽트 4, 6을 허용
	EIFR = 0x50;  // 인터럽트 4, 6 플래그를 클리어
	
	sei(); // 전체 인터럽트를 허용
	
	while (1)
	{
		PORTA = FND_DATA_TBL[cnt];
		if(Time_STOP == 0)
		{
			cnt++;
			if(cnt>17) cnt=0;
		}
		_delay_ms(200);
	}
}

SIGNAL(INT4_vect) // 인터럽트 서비스 루틴
{
	cli(); // 전체 인터럽트를 금지
	
	Time_STOP = 1;
	cnt = 0;
	
	sei();
}

SIGNAL(INT6_vect) // 인터럽트 서비스 루틴
{
	cli(); // 전체 인터럽트를 금지
	
	Time_STOP = 0;

	sei();
}
*/


// 인터럽트로 LED 점멸
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned char Time_STOP = 0;

int main(void)
{
	unsigned char LED_Data = 0x01;
	
	DDRC = 0x0F; // LED
	DDRD = 0xF0; // 스위치
	EICRA = 0x03;
	EIMSK = 0x01;
	EIFR = 0x01;
	sei();
	
	while (1)
	{
		PORTC = LED_Data;
		if(Time_STOP == 0)
		{
			if(LED_Data == 0x08) LED_Data = 0x01;
			else LED_Data <<= 1;
		}
		_delay_ms(100);
	}
}

SIGNAL(INT0_vect)
{
	cli();
	if(Time_STOP == 0)
	{
		Time_STOP = 1;
	}
	else
	{
		Time_STOP = 0;
	}
	sei();
}
