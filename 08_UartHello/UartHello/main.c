/*
 * UartHello.c
 *
 * Created: 2020-06-17 오후 3:07:33
 * Author : PKNU
 */ 
/*
// UART로 Hello 보내기
#include <avr/io.h>

void putch(unsigned char data)
{
	while((UCSR0A & 0x20) == 0); // 전송준비가 될 때까지 대기
	UDR0 = data; // 데이터를 UDR0에 쓰면 전송
	UCSR0A |= 0x20;
}

int main(void)
{
    unsigned char text[] = "Hello! World!! \r\n";
	unsigned char i = 0;
	
	DDRE = 0xfe; // Rx(입력 0), Tx(출력, 1)
	
	UCSR0A = 0x00;
	UCSR0B = 0x18;
	UCSR0C = 0x06;
	
	UBRR0H = 0x00;
	UBRR0L = 0x03; // 115200bps
	
    while (text[i]!='\0') // 문자열 데이터가 '\0'인 경우, 문자열 끝
    {
		putch(text[i++]); // 저장된 문자열 출력
    }
}
*/

/*
// UART로 스위치 입력 값 보내기
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TX_flag = 0;
volatile char TX_data = 0; // 스위치 전송 값 저장 변수

void putch(unsigned char data)
{
	while((UCSR0A & 0x20) == 0); // 전송준비가 될때까지 대기
	UDR0 = data; // 데이터를 UDR0에 쓰면 전송된다
	UCSR0A |= 0x20;
}

void putch_Str(char *str)
{
	unsigned char i = 0;
	while(str[i]!='\0')
		putch(str[i++]); // 문자열을 출력
}

int main(void){
	DDRE = 0x02; // 스위치 0~3 입력
	UCSR0A = 0x00;
	UCSR0B = 0x18;
	UCSR0C = 0x06;
	UBRR0H = 0x00;
	UBRR0L = 0x03;
	
	EICRB = 0xFF; // 인터럽트 4~7을 상승엣지에서 동작하도록 설정
	
	EIMSK = 0xF0; // 허용
	EIFR = 0xF0; // 플래그를 클리어
	sei();
	
	while(1)
	{
		if(TX_flag == 1)
		{
			putch_Str("\n\r Input Switch : ");
			putch(TX_data);
			TX_flag = 0;
		}
	}
}

SIGNAL(INT4_vect)
{
	cli();
	TX_data = '0'; // 문자 0을 저장
	TX_flag = 1;
	sei();
}

SIGNAL(INT5_vect)
{
	cli();
	TX_data = '1'; // 문자 1을 저장
	TX_flag = 1;
	sei();
}

SIGNAL(INT6_vect)
{
	cli();
	TX_data = '2'; // 문자 2를 저장
	TX_flag = 1;
	sei();
}

SIGNAL(INT7_vect)
{
	cli();
	TX_data = '3'; // 문자 3을 저장 
	TX_flag = 1;
	sei();
}
*/
/*
// UART로 PC와 데이터 주고받기
#include <avr/io.h>

void putch(unsigned char data){
	while((UCSR0A & 0x20) == 0); // 전송 준비가 될 때까지 대기
	UDR0 = data; // 데이터를 UDR0에 쓰면 전송
	UCSR0A |= 0x20;
}

unsigned char getch(){
	unsigned char data;
	while((UCSR0A & 0x80) == 0); // 데이터를 받을 때까지 대기
		data = UDR0; // 수신된 데이터는 UDR0에 저장
	UCSR0A |= 0x80;
	return data; // 읽어온 문자를 반환
}

int main(void)
{
	unsigned char text[] = "\r\nWelcome! edgeLAB \r\n USART 0 Program. \r\n";
	
	unsigned char echo[] = "ECHO >> ";
	unsigned char i = 0;
	
	DDRE = 0xfe;
	
	UCSR0A = 0x00;
	UCSR0B = 0x18;
	UCSR0C = 0x06;
	
	UBRR0H = 0x00;
	UBRR0L = 0x03;
	
	while(text[i]!='\0'){ // 문자열 데이터가 '\0'인 경우, 문자열 끝임
		putch(text[i++]); // 저장된 text 문자열 출력
	}
	i = 0; // 카운터변수 초기화
	while(echo[i]!='\0'){ // 문자열 데이터가 '\0'인 경우, 문자열 끝임
		putch(echo[i++]); // 저장된 echo 문자열을 출력
	}
	while(1){
		putch(getch()); // getch()로 입력받은 문자를 다시 putch()로 문자열 출력
	}
}
*/
/*
// UART로 PC에서 FND 점멸시키기
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TX_flag = 0;
volatile char TX_data = 0; // 스위치 전송 값 저장 변수

unsigned char FND_DATA_TBL [] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7C,0x07,0x7F,
								0x67,0x77,0x7C,0x39,0x5E,0x79,0x71,0x08,0x80}; // 0~9, A~F, _ .

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

int main(void)
{
	unsigned char RX_data = 0;
	DDRE = 0x0e; 
	DDRA = 0xFF; // FND
	
	UCSR0A = 0x00;
	UCSR0B = 0x18;
	UCSR0C = 0x06;
	
	UBRR0H = 0x00;
	UBRR0L = 0x03;
	
	while(1){
		RX_data = getch(); // PC로부터 입력 받은 데이터를 변수 RX_data에 저장
		
		if((RX_data >= 0x30) && (RX_data <= 0x39))
		{
			PORTA = FND_DATA_TBL[RX_data - 0x30];
			// 포트A에 입력된 값의 FND Table 값을 출력한다.
			// 아스키코드 '0'은 0x30
		}
	}
}
*/

// UART로 PC에서 숫자 입력받아 부저울리기
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned char TX_flag = 0;
volatile char TX_data = 0; // 스위치 전송 값 저장 변수

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

int main(void)
{
	unsigned char text[] = "\r\n +++ MENU +++ \r\n L : LED \r\n C : CDS \r\n 0 ~ 7 : PIANO \r\n S : Step Motor \r\n push button : stop \r\n ";
	unsigned char echo[] = "ECHO >> ";
	
	unsigned char i = 0;
	
	unsigned char RX_data = 0;
	DDRE = 0xfe;
	
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
		
		if((RX_data >= 0x30) && (RX_data <= 0x39))
		{
			TCCR1A |= 0x0A;
			ICR1 = 7372800/DoReMi[RX_data - 0x30];
			OCR1C = ICR1/10;
			_delay_ms(500);
			TCCR1A = 0x00; // 부저소리 끔
			ICR1 = 10;
			OCR1C = ICR1/10;
		}
		else
		{
			TCCR1A = 0x00; // 부저소리 끔
		}
	}
}

/*
// 피아노 잘안됨
#include <avr/io.h>
#include <util/delay.h>

unsigned int DoReMi[8] = {523, 587, 659, 698, 783, 880, 987, 1046};

void putch(unsigned char data){
	while((UCSR0A & 0x20) == 0);
	UDR0 = data;
	UCSR0A |= 0x20;
}

unsigned char getch(){
	unsigned char data;
	while((UCSR0A & 0x80) == 0);
	data = UDR0;
	UCSR0A |= 0x80;
	return data;
}


int main(void)
{
	unsigned char piano = 0;
	unsigned char text[] = "\r\nPlease Play Piano.(1~8)\r\n";

	unsigned char echo[] = "ECHO >> ";
	unsigned char i = 0;

	DDRE = 0xfe;
	DDRB = 0x80;

	UCSR0A = 0x00;
	UCSR0B = 0x18;
	UCSR0C = 0x06;

	UBRR0H = 0x00;
	UBRR0L = 0x03;
	
	TCCR1A |= 0x00;
	TCCR1B |= 0x19;
	TCCR1C = 0x00;
	TCNT1 = 0x0000;

	while(text[i]!='\0'){
		putch(text[i++]);
	}

	i = 0;

	while(echo[i]!='\0'){
		putch(echo[i++]);
	}

	while (1)
	{
		putch(getch());

		if(getch()=='1'){
			
			TCCR1A |= 0x0A;
			ICR1 = 7372800/DoReMi[0];
			OCR1C = ICR1/10;
			_delay_ms(500);
			TCCR1A |= 0x00;
			ICR1 = 10;
			OCR1C = ICR1/10;
		}

		else if(getch()=='2'){
			TCCR1A |= 0x0A;
			ICR1 = 7372800/DoReMi[1];
			OCR1C = ICR1/10;
			_delay_ms(500);
			TCCR1A |= 0x00;
			ICR1 = 10;
			OCR1C = ICR1/10;
		}

		else if(getch()=='3'){
			TCCR1A |= 0x0A;
			ICR1 = 7372800/DoReMi[2];
			OCR1C = ICR1/10;
			_delay_ms(500);
			TCCR1A |= 0x00;
			ICR1 = 10;
			OCR1C = ICR1/10;
		}

		else if(getch()=='4'){
			TCCR1A |= 0x0A;
			ICR1 = 7372800/DoReMi[3];
			OCR1C = ICR1/10;
			_delay_ms(500);
			TCCR1A |= 0x00;
			ICR1 = 10;
			OCR1C = ICR1/10;
		}

		else if(getch()=='5'){
			TCCR1A |= 0x0A;
			ICR1 = 7372800/DoReMi[4];
			OCR1C = ICR1/10;
			_delay_ms(500);
			TCCR1A |= 0x00;
			ICR1 = 10;
			OCR1C = ICR1/10;
		}

		else if(getch()=='6'){
			TCCR1A |= 0x0A;
			ICR1 = 7372800/DoReMi[5];
			OCR1C = ICR1/10;
			_delay_ms(500);
			TCCR1A |= 0x00;
			ICR1 = 10;
			OCR1C = ICR1/10;
		}

		else if(getch()=='7'){
			TCCR1A |= 0x0A;
			ICR1 = 7372800/DoReMi[6];
			OCR1C = ICR1/10;
			_delay_ms(500);
			TCCR1A |= 0x00;
			ICR1 = 10;
			OCR1C = ICR1/10;
		}

		else if(getch()=='8'){
			TCCR1A |= 0x0A;
			ICR1 = 7372800/DoReMi[7];
			OCR1C = ICR1/10;
			_delay_ms(500);
			TCCR1A |= 0x00;
			ICR1 = 10;
			OCR1C = ICR1/10;
		}
	}
}
*/