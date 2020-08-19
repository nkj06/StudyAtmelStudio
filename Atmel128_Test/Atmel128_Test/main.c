/*
+++ Memu +++
L : LED (좌우로 약 500ms 간격으로 이동)
C : CDS (CDS의 ADC 변환 값을 터미널에 출력)
0 ~ 7 : PIANO (숫자키별로 음계발생)
S : Step Motor (정방향 : f, 역방향 : r)
push button : stop (kit의 푸쉬버튼을 누르면 동작이 멈춘다.(인터럽트))
 */ 

// UART로 PC에서 숫자 입력받아 부저울리기
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned char TX_flag = 0;
volatile char TX_data = 0; // 스위치 전송 값 저장 변수

volatile unsigned char Time_STOP = 0; // 인터럽트

unsigned int DoReMi[8] = {523, 587, 659, 698, 783, 880, 987, 1046}; // 부저

void putch(unsigned char data)
{
	while((UCSR0A & 0x20) == 0); // 전송준비가 될때까지 대기
	UDR0 = data; // 데이터를 UDR0에 쓰면 전송된다
	UCSR0A |= 0x20;
}

unsigned char getch(void)
{
	unsigned char data;
	while((UCSR0A & 0x80)==0); // 데이터를 받을때까지 대기
	data = UDR0; // 수신된 데이터는 UDR0에 저장
	UCSR0A |= 0x80;
	return data; // 읽어온 문자를 반환
}

void putch_Str(char *str) // CDS값 출력할 거
{
	unsigned char i = 0;
	while(str[i]!='\0')
	putch(str[i++]); // 문자열을 출력
}

int main(void)
{
	unsigned char text[] = "\r\n +++ MENU +++ \r\n L : LED \r\n C : CDS \r\n 0 ~ 7 : PIANO \r\n [Step Motor] r : right, l : left \r\n push button 0 : stop \r\n ";
	unsigned char echo[] = "ECHO >> ";
	
	unsigned char i = 0, s = 0;
	
	unsigned char RX_data = 0; // PC에서 받아온 값을 넣을 변수
	
	unsigned char LED_Data = 0x01, l; // LED
	
	unsigned int AdData = 0; // CDS

	DDRC = 0xFF; // LED
	
	DDRE = 0xfe; // UART	
	UCSR0A = 0x00;
	UCSR0B = 0x18;
	UCSR0C = 0x06;
	UBRR0H = 0x00;
	UBRR0L = 0x03;
	
	DDRB = 0x80; // Buzzer를 PB7에 연결
	TCCR1A = 0x00;
	TCCR1B = 0x19;
	TCCR1C = 0x00;
	TCNT1 = 0x0000; // 타이머3 카운터 초기화
	
	DDRF = 0x02; // 광센서
	PORTF = 0x02;
	ADMUX = 0x00; // ADC0 선택
	ADCSRA = 0x86; // ADC 허가, 128분주
	
	DDRA = 0x20; // 스텝 모터
	DDRD = 0xF0; 
	PORTA &= ~0x20;

	// 스위치 인터럽트
	EICRA = 0x03;
	EIMSK = 0x01;
	EIFR = 0x01;
	sei();

	while(text[i]!='\0'){ // 문자열 데이터가 '\0'인 경우, 문자열 끝임
		putch(text[i++]); // 저장된 text 문자열 출력
	}
	i = 0; // 카운터변수 초기화
	while(echo[i]!='\0'){ // 문자열 데이터가 '\0'인 경우, 문자열 끝임
		putch(echo[i++]); // 저장된 echo 문자열을 출력
	}
	while(1){
		RX_data = getch(); // PC로부터 입력 받은 데이터를 변수 RX_data에 저장
		putch(RX_data);

		if((RX_data >= 0x30) && (RX_data <= 0x37)) // 피아노 0 ~ 7
		{
			putch_Str(" piano \n\r");
			TCCR1A |= 0x0A;
			ICR1 = 7372800/DoReMi[RX_data - 0x30];
			OCR1C = ICR1/10;
			_delay_ms(500);
			TCCR1A = 0x00; // 부저소리 끔
			ICR1 = 10;
			OCR1C = ICR1/10;
		}
		else if (RX_data == 'L') // LED
		{
			while(1)
			{
				if(s == 1)
				{
					s = 0;
					break;
				}
				LED_Data = 0x01;
				for(l = 0; l<8; l++){
					if(Time_STOP == 1)
					{
						s = 1;
						break;
					}
					PORTC = LED_Data;
					LED_Data <<= 1;
					_delay_ms(500);
				}
				LED_Data = 0x40;
				for(l=1; l<8; l++){
					if(Time_STOP == 1)
					{
						s = 1;
						Time_STOP = 0;
						PORTC = 0x00;
						break;
					}
					PORTC = LED_Data;
					LED_Data >>= 1;
					_delay_ms(500);
				}
			}
		}
		else if (RX_data == 'C') // CDS
		{
			while(1)
			{
				if(Time_STOP == 1)
				{
					Time_STOP = 0;
					break;
				}
				ADCSRA |= 0x40;
				while((ADCSRA & 0x10) == 0x00);
				AdData = ADC;
		//		sprintf(s," Cds: %d", AdData);
				putch_Str("\n\r CDS : ");
				putch((AdData/1000)%10 + '0');
				putch((AdData/100)%10 + '0');
				putch((AdData/10)%10 + '0');
				putch((AdData)%10 + '0');
				_delay_ms(500);
			}
		}
		else if (RX_data == 'r') // 오른쪽
		{		
			putch_Str("\n\r right ");
			while(1){
				if(Time_STOP == 1)
				{
					Time_STOP = 0;
					break;
				}
				PORTD = 0x30;
				_delay_ms(10);
				PORTD = 0x90;
				_delay_ms(10);
				PORTD = 0xC0;
				_delay_ms(10);
				PORTD = 0x60;
				_delay_ms(10);
			}
		}
		else if (RX_data == 'l') // 왼쪽
		{
			putch_Str("\n\r left ");
			while(1){
				if(Time_STOP == 1)
				{
					Time_STOP = 0;
					break;
				}
				PORTD = 0x10;
				_delay_ms(10);
				PORTD = 0x20;
				_delay_ms(10);
				PORTD = 0x40;
				_delay_ms(10);
				PORTD = 0x80; 
				_delay_ms(10);
			}
		}
	}
}

SIGNAL(INT0_vect)
{
	cli();
	if(Time_STOP == 0)
	{
		Time_STOP = 1;
		putch_Str("\n\r Stop ");
	}
	else
	{
		Time_STOP = 0;
	}
	sei();
}
