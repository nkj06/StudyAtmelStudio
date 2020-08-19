/*
 * MEMORY_Example.c
 *
 * Created: 2020-06-15 오후 12:27:07
 * Author : PKNU
 */ 

#include <avr/io.h>

unsigned char A = 0xAA;
unsigned char B = 0xBB;
unsigned char C = 0xCC;
unsigned char *ADD = 0x130;
int main(void)
{
    DDRA = 0xFF;
	PORTA = 0x00;
	DDRF = 0xFF;
	PORTF = 0x00;
	ADD = 0x140;
	*ADD = 0x11;
	*(ADD + 1) = 0x22;
	*(ADD + 2) = 0x33;
	*(ADD + 3) = 0x44;
	
	A = 0xBB;
	B = C - A;
	
	PORTA = A;
	PORTB = B;
	
    while (1) 
    {
		
    }
}

