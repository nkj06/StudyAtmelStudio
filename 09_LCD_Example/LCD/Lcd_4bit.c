/*
 * Lcd_4bit.c
 *
 * Created: 2017-07-09 ���� 10:44:06
 *  Author: CastleKing
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include "Lcd.h"

void LcdCmd_4bit(Byte ch)
{
	/* ���� 4bit ���� */
	LCD_CTRL &= ~(1 << LCD_RS); // RS == 0 : Instruction
	LCD_CTRL &= ~(1 << LCD_RW);	// RW == 0 : Write
	LCD_INSTWR = (ch & 0xf0) | (1 << LCD_EN);
	_delay_us(50);
	LCD_CTRL &= ~(1 << LCD_EN);
	/* ���� 4bit ���� */
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
	_delay_us(50);				// BF = 0 �� �� �ڿ� ��������� �� �� �ִ�. ���� delay�� �ش�.
	LCD_CTRL &= ~(1 << LCD_EN);	// Lcd Disable
	
	LCD_CTRL &= ~(1 << LCD_RW); // RW == 0
	LCD_DATAWR = ((ch << 4) & 0xf0) | (1 << LCD_RS) | (1 << LCD_EN);
	_delay_us(50);				// BF = 0 �� �� �ڿ� ��������� �� �� �ִ�. ���� delay�� �ش�.
	LCD_CTRL &= ~(1 << LCD_EN);	// Lcd Disable
}

void Lcd_CHAR(Byte c)			//������� �Լ�
{
	LcdData_4bit(c);
	_delay_ms(1);
}

void Lcd_STR(Byte *str)			//���ڿ� ��� �Լ�
{
	while(*str != 0){
		Lcd_CHAR(*str);
		str++;
	}
}

void Lcd_Pos(Byte col, Byte row)	//��ġ���� �Լ�
{
	LcdCmd_4bit(0x80 | (row + col * 0x40));
}

void Lcd_Clear(void)			//ȭ�� ���� �Լ�
{
	LcdCmd_4bit(0x01);
	_delay_ms(2);
}

void Lcd_Shift(Byte p)			//�̵� �Լ�
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
	_delay_ms(2);		//��ɾ IR�� ������ 37us �Ǵ� 1.52ms�� ��ٸ�.
	LcdCmd_4bit(0x28);	//��� �缳��
	_delay_us(37);
	LcdCmd_4bit(0x0c);	//LCD ON, Ŀ�� ������ ����
	_delay_us(37);
	LcdCmd_4bit(0x06);	//��Ʈ��:��� �̵�
	_delay_us(37);
	Lcd_Clear();
}
/*
int main(void)
{
	Byte str[]= "KKaeng_ji";
	Byte str1[] = "Hi";
	DDRA = 0xFF;
	LcdInit_4bit(); // LCD �ʱ�ȭ
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
// ����ġ ������ ������ LCD�� ���°�
int main(void)
{
	unsigned char Switch_flag = 0;
	
	int i = 0, j;
	char s[20];
	DDRA = 0xFF; // LCD
	DDRD = 0x00; // ��ư
	
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
			Byte str2[] = " number "; // lcd ù��°�� ���
			Lcd_Pos(0,0);
			Lcd_STR(str2);
		}

		if(i > 1)
		{
			for (j=1; j<10; j++)
			{
				Lcd_Clear(); // lcd ȭ�� �ʱ�ȭ
				Byte str[] = " GUGUDAN "; // lcd ù��°�� ���
				Lcd_Pos(0,0);
				Lcd_STR(str);
				
				Lcd_Pos(1,0); // lcd �ι�° �� ���
				sprintf(s, "%d * %d = %d", i, j, i*j);
				Lcd_STR(s);
				_delay_ms(1000);
			}
			Lcd_Clear(); // lcd ȭ�� �ʱ�ȭ
		}
	}
}
*/

// A/D �����ͷ� ������ �б� 1
int main(void)
{
	unsigned int AdData = 0;
	
	char s[20];
	
	DDRA = 0xFF; // LCD
	DDRF = 0x02;
	PORTF = 0x02;
	
	LcdInit_4bit(); // LCD�� �ʱ�ȭ
	
	ADMUX = 0x00; // ADC0 ����
	ADCSRA = 0x86; // ADC �㰡, 128����
	
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
