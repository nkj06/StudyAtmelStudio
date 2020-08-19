/*
 * SWITCH_Application.c
 *
 * Created: 2020-06-15 오전 10:23:53
 * Author : PKNU
 */ 

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	unsigned char Switch_flag = 0, Switch_flag_pre = 0x01;
	
	DDRA = 0x0F; // LED 출력포트
	DDRE = 0x00; // 버튼 입력포트
	
	PORTA = 0x00; // 버튼 초기값 OFF
	
	while (1)
	{
		Switch_flag = PINE >> 4;
		
		if((Switch_flag == 0x01) || (Switch_flag == 0x02) || (Switch_flag == 0x04) || (Switch_flag == 0x08))
		{
			Switch_flag_pre = Switch_flag;
		}
		
		PORTA ^= 0x0F;
		
		if(Switch_flag_pre == 0x01) // 버튼 4
			_delay_ms(250);
		else if(Switch_flag_pre == 0x02) // 5
		_delay_ms(500);
		else if(Switch_flag_pre == 0x04) // 6
		_delay_ms(750);
		else if(Switch_flag_pre == 0x08) // 7
		_delay_ms(1000);
		
	}
}

