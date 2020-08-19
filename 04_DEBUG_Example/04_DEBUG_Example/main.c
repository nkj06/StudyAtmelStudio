/*
 * DEBUG_Example.c
 *
 * Created: 2020-06-15 오후 12:09:47
 * Author : PKNU
 */ 

#include <avr/io.h>

unsigned char data = 0x01;

int main(void)
{
	unsigned long i;
	DDRA = 0xFF;
	PORTA = 0x00;
	
    while (1) 
    {
		PORTA = data;
		data <<=1;
		for(i=0; i<1000; i++);
		if(data == 0x00)
			data = 0x01;
    }
}