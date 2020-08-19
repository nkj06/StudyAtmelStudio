/*
 * Lcd_4bit.c
 *
 * Created: 2017-07-09 오후 10:44:06
 *  Author: CastleKing
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include "Lcd.h"

void LcdCmd_4bit(Byte ch)
{
	/* 상위 4bit 전송 */
	LCD_CTRL &= ~(1 << LCD_RS); // RS == 0 : Instruction
	LCD_CTRL &= ~(1 << LCD_RW);	// RW == 0 : Write
	LCD_INSTWR = (ch & 0xf0) | (1 << LCD_EN);
	_delay_us(50);
	LCD_CTRL &= ~(1 << LCD_EN);
	/* 하위 4bit 전송 */
	LCD_CTRL &= ~(1 << LCD_RS); // RS == 0
	LCD_CTRL &= ~(1 << LCD_RW);	// RW == 0
	LCD_INSTWR = ((ch << 4) & 0xf0)	| (1 << LCD_EN);
	_delay_us(50);
	LCD_CTRL &= ~(1 << LCD_EN);
}

void LcdData_4bit(Byte ch)
{
	LCD_CTRL &= ~(1 << LCD_RW); // RW == 0
	LCD_DATAWR = (ch & 0xf0) | (1 << LCD_RS) | (1 << LCD_EN);
	_delay_us(50);				// BF = 0 이 된 뒤에 다음명령을 쓸 수 있다. 따라서 delay를 준다.
	LCD_CTRL &= ~(1 << LCD_EN);	// Lcd Disable
	
	LCD_CTRL &= ~(1 << LCD_RW); // RW == 0
	LCD_DATAWR = ((ch << 4) & 0xf0) | (1 << LCD_RS) | (1 << LCD_EN);
	_delay_us(50);				// BF = 0 이 된 뒤에 다음명령을 쓸 수 있다. 따라서 delay를 준다.
	LCD_CTRL &= ~(1 << LCD_EN);	// Lcd Disable
}

void Lcd_CHAR(Byte c)			//문자출력 함수
{
	LcdData_4bit(c);
	_delay_ms(1);
}

void Lcd_STR(Byte *str)			//문자열 출력 함수
{
	while(*str != 0){
		Lcd_CHAR(*str);
		str++;
	}
}

void Lcd_Pos(Byte col, Byte row)	//위치지정 함수
{
	LcdCmd_4bit(0x80 | (row + col * 0x40));
}

void Lcd_Clear(void)			//화면 지움 함수
{
	LcdCmd_4bit(0x01);
	_delay_ms(2);
}

void Lcd_Shift(Byte p)			//이동 함수
{
	if(p == RIGHT){
		LcdCmd_4bit(0x1c);
		_delay_ms(1);
		}else if(p == LEFT){
		LcdCmd_4bit(0x18);
		_delay_ms(1);
	}
}

void LcdInit_4bit(void)
{
	LcdCmd_4bit(0x02);	//cursor Home
	_delay_ms(2);		//명령어를 IR에 쓰고나면 37us 또는 1.52ms을 기다림.
	LcdCmd_4bit(0x28);	//기능 재설정
	_delay_us(37);
	LcdCmd_4bit(0x0c);	//LCD ON, 커서 깜박임 없음
	_delay_us(37);
	LcdCmd_4bit(0x06);	//엔트리:우로 이동
	_delay_us(37);
	Lcd_Clear();
}
/*
int main(void)
{
	Byte str[]= "KKaeng_ji";
	Byte str1[] = "Hi";
	DDRA = 0xFF;
	LcdInit_4bit(); // LCD 초기화
	while (1)
	{
		Lcd_Pos(0,0);
		Lcd_STR(str);
		Lcd_Pos(1,0);
		Lcd_STR(str1);
	}
}
*/
/*
// 스위치 누르면 구구단 LCD에 띄우는거
int main(void)
{
	unsigned char Switch_flag = 0;
	
	int i = 0, j;
	char s[20];
	DDRA = 0xFF; // LCD
	DDRD = 0x00; // 버튼
	
	LcdInit_4bit();
	
	while(1)
	{
		Switch_flag = PIND;
		
		if (Switch_flag == 0x01) // 3
			i = 8;
		else if (Switch_flag == 0x02) // 3
			i = 9;
		else if(Switch_flag == 0x04) // 4
			i = 2;
		else if(Switch_flag == 0x08) // 5
			i = 3;
		else if(Switch_flag == 0x10) // 6
			i = 4;
		else if(Switch_flag == 0x20) // 7
			i = 5;
		else if(Switch_flag == 0x40) // 8
			i = 6;
		else if(Switch_flag == 0x80) // 9
			i = 7;
		else
		{
			i = 0;
			Byte str2[] = " number "; // lcd 첫번째줄 출력
			Lcd_Pos(0,0);
			Lcd_STR(str2);
		}

		if(i > 1)
		{
			for (j=1; j<10; j++)
			{
				Lcd_Clear(); // lcd 화면 초기화
				Byte str[] = " GUGUDAN "; // lcd 첫번째줄 출력
				Lcd_Pos(0,0);
				Lcd_STR(str);
				
				Lcd_Pos(1,0); // lcd 두번째 줄 출력
				sprintf(s, "%d * %d = %d", i, j, i*j);
				Lcd_STR(s);
				_delay_ms(1000);
			}
			Lcd_Clear(); // lcd 화면 초기화
		}
	}
}
*/

// A/D 컨버터로 광센서 읽기 1
int main(void)
{
	unsigned int AdData = 0;
	
	char s[20];
	
	DDRA = 0xFF; // LCD
	DDRF = 0x02;
	PORTF = 0x02;
	
	LcdInit_4bit(); // LCD를 초기화
	
	ADMUX = 0x00; // ADC0 선택
	ADCSRA = 0x86; // ADC 허가, 128분주
	
	while (1)
	{
		ADCSRA |= 0x40;
		while((ADCSRA & 0x10) == 0x00);
		AdData = ADC;
		Lcd_Pos(0,0);
		
		sprintf(s," Cds: %d", AdData);
		Lcd_STR(s);
		
		_delay_ms(500);
		Lcd_Clear();
	}
}
