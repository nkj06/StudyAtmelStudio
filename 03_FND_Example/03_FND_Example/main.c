/*
 * FND_Example.c
 *
 * Created: 2020-06-15 오전 11:07:09
 * Author : PKNU
 */ 
/*
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB = 0xff;
    while (1) 
    {
		PORTB = 0b00111111; // 0x3f
		_delay_ms(1000);
		PORTB = 0b00000110; // 0x06
		_delay_ms(1000);
    }
}
*/
/*
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	unsigned char FND_DATA_TBL [] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7C,0x07,0x7F,
									0x67,0x77,0x7C,0x39,0x5E,0x79,0x71,0x08,0x80};

	unsigned char cnt = 0;
	DDRB = 0xFF;
	while(1){
		PORTB = FND_DATA_TBL[cnt];
		cnt++;
		if(cnt>17) cnt=0;
		_delay_ms(500);
	}
}
*/

/*
#include <avr/io.h>
#include <util/delay.h>

int Count_TR(unsigned char flag);

int main(void)
{
	unsigned char FND_DATA_TBL [] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7C,0x07,0x7F,
									0x67,0x77,0x7C,0x39,0x5E,0x79,0x71,0x08,0x80}; // 0~9, A~F
									
	unsigned int cnt = 0;
	unsigned char Switch_flag = 0;
	
	DDRB = 0xFF; // FND
	DDRE = 0x00; // 스위치
	// 4는 +1 5는 +2 6는 -1 7은 -2
	
	while(1){
		Switch_flag = PINE >> 4; // 입력핀이 포트E의 상위 비트이므로 우측으로 4비트 쉬프트
		while(PINE >> 4 != 0x00); // 스위치를 눌렀을 경우 땔 때까지 대기
		
		if(Switch_flag != 0) // 스위치가 눌렀을 경우에 만 실행
		cnt += Count_TR(Switch_flag);
		
		if(cnt < 0)
			cnt = 0;
		else if(cnt > 15)
			cnt = 15;
			
		PORTB = FND_DATA_TBL[cnt];
		_delay_ms(100);
	}
}

int Count_TR(unsigned char flag){
	int count = 0;
	
	switch (flag) {
	case 0x01:
		count = 1;
		break;
	case 0x02:
		count = 2;
		break;
	case 0x04:
		count = -1;
		break;
	case 0x08:
		count = -2;
		break;
	}
	return count;
}

*/

#include <avr/io.h>
#include <util/delay.h>

int Count_TR(unsigned char flag);

int main(void)
{
	unsigned char FND_DATA_TBL [] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7C,0x07,0x7F,
	0x67,0x77,0x7C,0x39,0x5E,0x79,0x71,0x08,0x80}; // 0~9, A~F
	
	unsigned int cnt = 0;
	unsigned char Switch_flag = 0;
	
	DDRB = 0xFF; // FND
	DDRE = 0x00; // 스위치
	DDRC = 0x00; // 스위치
	
	while(1){
		Switch_flag = PINE >> 4; // 입력핀이 포트E의 상위 비트이므로 우측으로 4비트 쉬프트
		while(PINE >> 4 != 0x00); // 스위치를 눌렀을 경우 땔 때까지 대기
		
		if(Switch_flag != 0) // 스위치가 눌렀을 경우에 만 실행
		cnt = Count_TR(Switch_flag);

		PORTB = FND_DATA_TBL[cnt];
		_delay_ms(100);
	}
}

int Count_TR(unsigned char flag){
	int count = 0;
	
	switch (flag) {
		case 0x01:
			count = 0;
			break;
		case 0x02:
			count = 1;
			break;
		case 0x04:
			count = 2;
			break;
		case 0x08:
			count = 3;
			break;
		case 0x10: 
			count = 4;
			break;
		case 0x20:
			count = 5;
			break;
		case 0x40:
			count = 6;
			break;
		case 0x80:
			count = 7;
			break;

	}
	return count;
}