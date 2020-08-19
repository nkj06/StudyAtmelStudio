/*
 * 03_LED_Example.c
 *
 * Created: 2020-06-12 오후 4:23:54
 * Author : PKNU
 */ 
/*
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	unsigned char LED_Data = 0x00;
	DDRC = 0x0F;
	
    while (1) 
    {
		PORTC = LED_Data;
		LED_Data++;
		
		if(LED_Data > 0x0F) LED_Data = 0;
		_delay_ms(1000);
    }
}
*/

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	unsigned char LED_Data = 0x01, i;
	DDRC = 0x0F;
	
	while(1)
	{
		LED_Data = 0x01;
		
		for(i = 0; i<4; i++){
			PORTC = LED_Data;
			LED_Data <<= 1;
			_delay_ms(1000);
		}
		for(i=0; i<4; i++){
			PORTC = LED_Data;
			LED_Data >>= 1;
			_delay_ms(1000);
		}
	}
}