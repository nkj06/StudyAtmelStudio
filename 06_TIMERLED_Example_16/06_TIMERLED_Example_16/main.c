/*
 * 06_TIMERLED_Example_16.c
 *
 * Created: 2020-06-16 오전 11:37:14
 * Author : PKNU
 */ 

/*
// 타이머로 LED 점멸
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char LED_Data = 0x00;
unsigned char timer1Cnt=0;

int main(void)
{
    DDRC = 0x0F;
	
	TCCR1A = 0x00;
	TCCR1B = 0x01; // 프리스케일러 1
	
	TCNT1 = 0; // 0 -> 1/255초 마다 한번씩 인터럽트 발생
	TIMSK = 0x04;
	TIFR |=1 << TOV1;
	
	sei();
	
    while (1)
    {
		PORTC = LED_Data; // 포트C로 변수 LED_Data에 있는 데이터 출력
    }
	return 0;
}

SIGNAL(TIMER1_OVF_vect)
{
	cli();
	
	timer1Cnt++;
	
	if(timer1Cnt == 225)
	{
		LED_Data++;
		
		if(LED_Data > 0x0F)
			LED_Data = 0;
			
		timer1Cnt = 0;
	}
	sei();
}
*/

//프리스케일러를 변경하여 타이머로 LED 점멸(2)
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char LED_Data = 0x00;

int main(void)
{
	DDRC = 0x0F;
	
	TCCR3A = 0x00;
	TCCR3B = 0x05;
	
	TCNT3 = 58336;
	ETIMSK = 0x04;
	ETIFR |=1 << TOV3;
	
	sei();
	
	while(1){
		PORTC = LED_Data;
	}
	return 0;
}

SIGNAL(TIMER3_OVF_vect){
	cli();
	
	TCNT3 = 58336;
	LED_Data++;
	
	if(LED_Data>0x0F)
		LED_Data = 0;
	sei();
}