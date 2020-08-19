/*
 * 07_PwmLed_Example.c
 *
 * Created: 2020-06-17 오전 11:23:46
 * Author : PKNU
 */ 
/*
// PWM으로 LED 밝기 조절 하기
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	unsigned char Light = 0;
	
	DDRB = 0x10;
	
	TCCR0 = 0x77;
	TCNT0 = 0x00;
	
    while (1) 
    {
		for(Light=0; Light <255; Light++)
		{
			OCR0 = Light;
			_delay_ms(100);
		}
		for(Light=255; 0<Light; Light--)
		{
			OCR0 = Light;
			_delay_ms(100);
		}
    }
}
*/
/*
// 스위치 입력 값에 따라 LED 밝기 조절 하기
#include <avr/io.h>
#include <avr/interrupt.h>

volatile int Light = 0;
volatile unsigned char Light_flag = 1;

int main(void)
{
	DDRB = 0x80;
	
	DDRE = 0x00;
	
	TCCR2 = 0x62;
	TCNT2 = 0x00;
	
	EICRB = 0xFF;
	EIMSK = 0xF0;
	EIFR = 0xF0;
	
	sei();
	
	while(1)
	{
		if(Light_flag)
		{
			OCR2 = Light;
			Light_flag = 0;
		}
	}
}

SIGNAL(INT4_vect){
	cli();
	Light = 0;
	Light_flag = 1;
	sei();
}

SIGNAL(INT5_vect)
{
	cli();
	Light -= 51;
	if(Light < 0)
		Light = 0;
	Light_flag = 1;
	sei();
}

SIGNAL(INT6_vect)
{
	cli();
	Light += 51;
	if(Light > 255)
	Light = 255;
	Light_flag = 1;
	sei();
}

SIGNAL(INT7_vect)
{
	cli();
	Light = 255;
	Light_flag = 1;
	sei();
}
*/
/*
// PWM으로 PIEZO 울리기
#include <avr/io.h>
#include <util/delay.h>

unsigned int DoReMi[8] = {523, 587, 659, 698, 783, 880, 987, 1046};
	
int main(void)
{
	unsigned char piano = 0;
	
	DDRB = 0x80;
	
	TCCR1A |= 0x0A;
	TCCR1B |= 0x19;
	TCCR1C = 0x00;
	TCNT1 = 0x0000;
	
	while(1){
		ICR1 = 7372800/DoReMi[piano];
		OCR1C = ICR1/10;
		piano++;
		if(8<piano) piano = 0;
		_delay_ms(1000);
	}
}
*/
/*
// 스위치 입력 값에 따라 PIEZO 울리기
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned int DoReMi[8] = {523, 587, 659, 698, 783, 880, 987, 1046};
volatile unsigned char sound_flag = 1;

int main(void)
{
	DDRE = 0x08; // Buzzer를 PE3에 연결
	
	TCCR3A = 0x00;
	TCCR3B = 0x19;
	
	TCCR3C = 0x00; 
	TCNT3 = 0x0000; // 타이머3 카운터 초기화
	
	EICRA = 0xFF; // 버튼 0~3 상승엣지에서 동작
	EICRB = 0xFF; // 버튼 4~7 상승엣지에서 동작
	EIMSK = 0xFF; // 허용
	EIFR = 0xFF; // 플래그를 클리어
	sei();
	
	while(1)
	{
		if(sound_flag)
		{
			_delay_ms(1000);
			TCCR3A = 0x00; // 부저소리 끔
			sound_flag = 0;
		}
	}
}

SIGNAL(INT0_vect){
	cli();
	ICR3 = 7372800/DoReMi[0]/10; // 도
	TCCR3A = 0x40; // PE4로 출력
	sound_flag = 1; // 부저 음 발생
	sei();
}

SIGNAL(INT1_vect){
	cli();
	ICR3 = 7372800/DoReMi[1]/10;
	TCCR3A = 0x40;
	sound_flag = 1;
	sei();
}

SIGNAL(INT2_vect){
	cli();
	ICR3 = 7372800/DoReMi[2]/10;
	TCCR3A = 0x40;
	sound_flag = 1;
	sei();
}

SIGNAL(INT3_vect){
	cli();
	ICR3 = 7372800/DoReMi[3]/10;
	TCCR3A = 0x40;
	sound_flag = 1;
	sei();
}

SIGNAL(INT4_vect){
	cli();
	ICR3 = 7372800/DoReMi[4]/10;
	TCCR3A = 0x40;
	sound_flag = 1;
	sei();
}

SIGNAL(INT5_vect){
	cli();
	ICR3 = 7372800/DoReMi[5]/10;
	TCCR3A = 0x40;
	sound_flag = 1;
	sei();
}

SIGNAL(INT6_vect){
	cli();
	ICR3 = 7372800/DoReMi[6]/10;
	TCCR3A = 0x40;
	sound_flag = 1;
	sei();
}

SIGNAL(INT7_vect){
	cli();
	ICR3 = 7372800/DoReMi[7]/10;
	TCCR3A = 0x40;
	sound_flag = 1;
	sei();	
}
*/