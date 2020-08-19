/*
 * 11_Step1Phase_Example.c
 *
 * Created: 2020-06-18 오전 9:33:31
 * Author : PKNU
 */ 
/*
// 1상 여자 방식으로 스텝 모터 돌리기 (왼쪽으로 계속 돌아감)
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB = 0x20; // MOTOR1_EN을 출력 포트로 설정
	DDRD = 0xF0; // DDRD 4 ~ 7 - STEP 0 ~ 3을 출력 포트로 설정
	PORTB &= ~0x20; // M1 Disable, DC 모터 정지
	    
    while (1) 
    {
		PORTD = 0x10; // 1 Step
		_delay_ms(10);
		PORTD = 0x20; // 2 Step
		_delay_ms(10);
		PORTD = 0x40; // 3 Step
		_delay_ms(10);
		PORTD = 0x80; // 4 Step
		_delay_ms(10);
    }
}
*/
/*
// 2상 여자 방식으로 스텝 모터 돌리기 (오른쪽 한 바퀴돌고 멈추고)
#include <avr/io.h>
#include <util/delay.h>

int main(void){
	unsigned char i;
	
	DDRB = 0x20;
	DDRD = 0xF0;
	PORTB &= ~0x20;
	
	while(1){
		for(i = 0; i < 12; i++){
			PORTD = 0x30;
			_delay_ms(10);
			PORTD = 0x90;
			_delay_ms(10);
			PORTD = 0xC0;
			_delay_ms(10);
			PORTD = 0x60;
			_delay_ms(10);
		}
		_delay_ms(1000);
	}
}
*/
// 1-2상 여자 방식으로 스텝 모터 돌리기1 (다음 컬러까지가서 멈췄다가 다음 컬러)
#include <avr/io.h>
#include <util/delay.h>

unsigned char Step[8] = {0x90, 0x80, 0xC0, 0x40, 0x60, 0x20, 0x30, 0x10};

int main(void){
	unsigned char i, t = 0;
	
	DDRA = 0x20;
	DDRD = 0xF0;
	PORTA &= ~0x20;
	
	while(1){
		// 명령당 3.75도 회전, 90도 회전하려면 총 24개의 명령이 필요
		for(i = 0; i < 24; i++){ 
			PORTD = Step[t]; // 한 스텝 실행
			t++; 
			if(t>7) t=0; // 8 스텝을 초과하지 않도록 초기화
			_delay_ms(10);
		}
		_delay_ms(1000);
	}
}