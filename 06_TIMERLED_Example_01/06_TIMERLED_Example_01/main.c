/*
 * 06_TIMERLED_Example_01.c
 *
 * Created: 2020-06-15 오후 4:56:28
 * Author : PKNU
 */ 
/*
// 타이머로 LED 점멸
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char LED_Data = 0x00;
unsigned char timer0Cnt=0;
SIGNAL(TIMER0_OVF_vect);  // Timer0 Overflow0 ISP

int main(void)
{
    DDRC = 0x0F; // LED
	TCCR0 = 0x07;
	TCNT0 = 184; // 256-144 = 112 -> 0.01초 마다 한번씩 인터럽트 발생
	
	TIMSK = 0x01;
	TIFR |= 1 << TOV0;
	sei();
	
    while (1) 
    {
		PORTC = LED_Data; 
    }
}

SIGNAL(TIMER0_OVF_vect){
	cli();
	TCNT0 = 184;
	timer0Cnt++;
	if(timer0Cnt == 100){
		LED_Data++;
		if(LED_Data>0x0F) LED_Data = 0;
		timer0Cnt = 0;
	}
	sei();
}
*/
/*
// 타이머로 LED 점멸(2)
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char LED_Data = 0x01;
unsigned char timer2Cnt=0, Shift_Flag = 0;

SIGNAL(TIMER2_OVF_vect);

int main(void)
{
	DDRC = 0x0F; // LED
	
	TCCR2 = 0x05; // 프리스케일러 1024로 설정
	
	TCNT2 = 184; TIMSK = 0x40;
	TIFR |= 1 << TOV2;
	
	sei();
	
	while(1)
	{
		PORTC = LED_Data;
	}
	return 0;
}

SIGNAL(TIMER2_OVF_vect){
	cli();
	TCNT2 = 184;
	timer2Cnt++;
	
	if(timer2Cnt == 50){
		if(Shift_Flag == 0)
		{
			LED_Data <<= 1;
			if(LED_Data == 0x08)
				Shift_Flag = 1;
		}
		else
		{
			LED_Data >>= 1;
			if(LED_Data == 0x01)
				Shift_Flag = 0;
		}
		timer2Cnt = 0;
	}
	sei();
}
*/
/*
// 프리스케일러를 변경하여 타이머로 LED 점멸(1)
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char LED_Data = 0x00;
unsigned int timer0cnt=0;

SIGNAL(TIMER0_OVF_vect); //ISR(TIMER0_OVF_vect);

int main()
{
	DDRC = 0x0F;

	TCCR0 = 0x06;

	TCNT0 = 0xFF - 72;
	TIMSK = 0x01;
	TIFR |=1 << TOV0;

	sei();

	while(1)
	{
		PORTC = LED_Data;
	}
	
	return 0;
}

SIGNAL(TIMER0_OVF_vect)
{
	cli();

	TCNT0 = 183;
	timer0cnt++;

	if(timer0cnt == 400)
	{
		LED_Data++;
		if(LED_Data>0xFF) LED_Data = 0;
		timer0cnt=0;
	}

	sei();
}
*/

/*

// 타이머를 이용한 디지털 시계(1)

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char FND_DATA_TBL [] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7C,0x07,0x7F,
								0x67,0x77,0x7C,0x39,0x5E,0x79,0x71,0x08,0x80}; // 0~9, A~F, _
								
volatile unsigned char time_s=0;
unsigned char timer0Cnt=0;

int main(void)
{
	DDRA = 0xFF;
	
	TCCR0 = 0x07; // TCCR0 = (1 << CS02) | (1 << CSO1) | (1 < CS00);
	OCR0 = 72;
	TIMSK = 0x02;
	TIFR |=1 << OCF0;
	
	sei();
	
	while(1)
	{
		PORTA = FND_DATA_TBL[time_s];
	}
	return 0;
}

SIGNAL(TIMER0_COMP_vect)
{
	cli();
	OCR0 += 72;
	timer0Cnt++;
	
	if(timer0Cnt == 100)
	{
		if(time_s >= 16)
			time_s = 0;
		else time_s++;
			timer0Cnt=0;
	}
	sei();
}
*/
/*
// 타이머와 인터럽트로 FND 점멸(1)
#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char FND_DATA_TBL [] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7C,0x07,0x7F,
								0x67,0x77,0x7C,0x39,0x5E,0x79,0x71,0x08,0x80}; // 0~9, A~F, _

volatile unsigned char time_s=0;
volatile unsigned char Time_STOP = 0;

unsigned char timer0Cnt=0;

int main(void)
{
	DDRA = 0xFF;
	DDRE = 0x00;
	
	TCCR2 = 0x05;
	OCR2 = 72;
	TIMSK = 0x40;
	TIFR |=1 << OCF2;
	
	EICRB = 0x03;
	EIMSK = 0x10;
	EIFR = 0x10;
	
	sei();
	
	while(1)
	{
		PORTA = FND_DATA_TBL[time_s];
	}
	return 0;
}

SIGNAL(TIMER2_OVF_vect)
{
	cli();
	OCR2 += 72;
	timer0Cnt++;
	if(timer0Cnt == 100)
	{
		if(Time_STOP == 0)
		{
			if(time_s >= 16)
				time_s = 0;
			else
				time_s++;
		}
		timer0Cnt=0;
	}
	sei();
}

SIGNAL(INT4_vect) // 인터럽트 서비스 루틴
{
	cli();
	
	if(Time_STOP == 0)
		Time_STOP = 1;
	else
		Time_STOP = 0;
		
	sei();
}
*/

/*
// 타이머를 이용한 디지털 시계(2)
#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char FND_DATA_TBL [] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7C,0x07,0x7F,
								0x67,0x77,0x7C,0x39,0x5E,0x79,0x71,0x08,0x80}; // 0~9, A~F, _ .

volatile unsigned char time_s = 0;

int main(void) {
	DDRA = 0xFF;
	
	TCCR1A = 0x00; // 0b00000000
	TCCR1B = 0x05; // (1 << CS12) | (1 << CS10);
	
	OCR1A = 14400;
	TIMSK = 0x10; // 1 << OCIE1A
	TIFR |=1 << OCF1A;
	
	sei();
	
	while(1){
		PORTA = FND_DATA_TBL[time_s];
	}
	return 0;
}

SIGNAL(TIMER1_CAPT_vect){
	cli();
	
	OCR1A += 14400;
	if(time_s >= 17)
		time_s = 0
	else
		time_s++;
		
	sei();
}
*/
/*
//타이머를 이용한 스위치 누른시간 구하기
#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char FND_DATA_TBL[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f, 0x67, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x08, 0x80};

volatile unsigned char time_s=0;
volatile unsigned char FND_flag=0, edge_flag=0;

int main()
{
   DDRA = 0xFF;
   DDRE = 0x00;

   TCCR3A = 0; // 0x00
   TCCR3B = (1 << ICES3) | (1 << CS32) | (1 << CS30); // 0x45 // 프리스케일러 1024, 상승에지 캡처 트리거 설정
   ETIMSK = 1 << TICIE3; // 0x20
   ETIFR |= 1 << ICF3;

   sei();

   PORTA = FND_DATA_TBL[0];
   while(1)
   {
      if(FND_flag)
      {
         if(time_s > 15) time_s=15; // 최대 15초까지 표시(F)
         
         PORTA = FND_DATA_TBL[time_s];
         FND_flag=0;
      }
   }

   return 0;
}

SIGNAL(TIMER3_CAPT_vect)
{
   cli();

   unsigned int count_check;

   // 스위치가 눌린시간 측정을 위해 상승에지에서 하강에지까지의 시간 계산
   if(edge_flag==0) // 상승 에지(스위치를 누르면)
   {
      TCCR3B = (1 << CS32) | (1 << CS30); // 0x05 // 프리스케일러 1024, 하강에지 캡처 트리거 설정
      TCNT3 = 0; // TCNT3 레지스터 0으로 초기화
      ICR3 = 0; // ICR3 레지스터 0으로 초기화
      edge_flag=1;
   }
   else // 하강 에지
   {
      TCCR3B = (1 << ICES3) | (1 << CS32) | (1 << CS30); // 0x45 // 프리스케일러 1024, 상승에지 캡처 트리거 설정
      count_check = ICR3;

      // 7372800/1024 = 7200 Hz, 1초동안 TCNT의 값은 7200
      time_s = count_check/3600; // 누를 시간을 0.5초 단위로 변경, 1초는 7200

      FND_flag=1;
      edge_flag=0;
   }

   sei();
}
*/

