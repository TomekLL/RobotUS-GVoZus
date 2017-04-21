/*
 * IncFile1.h
 *
 * Created: 25.12.2016 21:00:00
 *  Author: Matúš Kavec
 */
#ifndef INCFILE1_H_
#define INCFILE1_H_

#define nula	0b11000000
#define jedna	0b11111001
#define dva	0b10100100
#define tri	0b10110000
#define styri	0b10011001
#define pat	0b10010010
#define sest	0b10000010
#define sedem	0b11111000
#define osem	0b10000000
#define devat	0b10010000
#define ahex	0b10001000
#define bhex	0b10000011
#define chex	0b10100111
#define dhex	0b10100001
#define ehex	0b10000110
#define fhex	0b10001110

#ifndef F_CPU
#define F_CPU	10000000UL	//definicia frekvencie CPU
#endif
#define BAUDERATE 9600
#define UBRR_VALUE ((F_CPU / (BAUDERATE * 16UL)) -1)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void setLED();
void setButton();
void setMotor();
void rightMotor(int, int);
void leftMotor(int, int);
void stop();
void setUSART(void);
int USART_send_1byte(char u8data);
int USART_receive_1byte(void);
void zobraz_7SEG(unsigned char zob);
void set_7SEG(void);
void ADC_1(uint8_t kanal);
uint8_t prevod();

#define sbi(x,y) x |= _BV(y)	//set bit- nastavenie bitu
#define cbi(x,y) x &= (~(_BV(y)))	//clear bit - nulovanie bitu
#define tbi(x,y) x ^=_BV(y)		//toggle bit - zmena bitu

#define is_high(y) (PINB & _BV(y) == _BV(y)) //=nestlačené kontrola, ci je nastaveny y-ty bit registra 'x'
#define is_low(y) ((PINB & _BV(y)) != _BV(y)) //=stlačené kontrola, ci je vstupny pin nastaveny na logicku nulu

void setLED()
{
	//****LED****
	DDRA |=(1<<PA5) | (1<<PA6) | (1<<PA7);
	sbi(PORTA,PA5);	
	sbi(PORTA,PA6);
	sbi(PORTA,PA7);
}
void setButton()
{
	//****Button****
	DDRB &= ~(1<<PB2);
	DDRB &= ~(1<<PB3);
	PORTB |=(1<<PB2) | (1<<PB3); //pull-up odpor
}
void zobraz_7SEG(unsigned char zob) // funkcia periodicky volana  kazdych 10ms
{									// zobrazuje na LED1 a LED2 hodnotu zob
	uint8_t zobrazovac;
	unsigned char vys;				// lokalna premenna uint_8, alebo char
	zobrazovac = PORTB & 0b00000011;
	if(zobrazovac==2) zob = zob & 0b00001111;
	else {zob = zob & 0b11110000; zob = zob >> 4; }
	switch(zob)
	{
		case 0: vys=nula; break;
		case 1: vys=jedna; break;
		case 2: vys=dva; break;
		case 3: vys=tri; break;
		case 4: vys=styri; break;
		case 5: vys=pat; break;
		case 6: vys=sest; break;
		case 7: vys=sedem; break;
		case 8: vys=osem; break;
		case 9: vys=devat; break;
		case 10: vys=ahex; break;
		case 11: vys=bhex; break;
		case 12: vys=chex; break;
		case 13: vys=dhex; break;
		case 14: vys=ehex; break;
		case 15: vys=fhex; break;
		default: vys=0;
	}
	PORTB = PORTB | 0b00000011; // zhasnu oba LED
	PORTC=vys;					// nastavi port C
	PORTB= PORTB & ~zobrazovac;	//zasvieti prislusny LED
}
void set_7SEG()
{
	DDRC=0xff;				// vystup katody zobrazovaca
	DDRB=0b00000011;		// ovladanie anod Z1 a Z2
	PORTB |=(1<<PB1);		// Z1 vypnuty Z0 zapnuty default
//***********************doplnok nastavenie TC0	
	TIMSK |=(1<<OCIE0);			// lokalne povolenie prerusenia
	OCR0= 98;	//kazdych 10 ms bude volana obsluha prerusenia TC0
	TCCR0=0b00001101;	//CTC rezim najpomalsie hodiny F_CPU/1024
}
void setMotor()
{
	DDRD |=(1<<PD4); //PD4=RYCHLOST_B
	DDRD |=(1<<PD5);//PD5=RYCHLOST_A
	
	DDRD |=(1<<PD6); //PD6= SMER_A
	DDRD |=(1<<PD7); //PD7= SMER_B
	
	TCCR1A = 0xa1;
	TCCR1B = 0xa;
}
void leftMotor(int smera, int speeda)
{
	if ((smera==1 || smera==0) && speeda<=255)
	{
		PORTD |=(smera<<PD7);		//Motor L dopredu
		OCR1BH = 0;			//MOTOR_L
		OCR1BL = speeda;		//Motor L rýchlosť
	}
}
void rightMotor(int smerb, int speedb)
{
	if ((smerb==1 || smerb==0) && speedb<=255)
	{
		PORTD |=(smerb<<PD6);		//Motor R dopredu=1, dozadu=0
		OCR1AH = 0;			//MOTOR R
		OCR1AL = speedb;		//Motor R rýchlosť
	}
}
void stop()
{
	OCR1AL = 0;
	OCR1BL = 0;
}
void setUSART(void)
{
	DDRD |=(1<<PD1); // Tx vystup
	UBRRH = (uint8_t)(UBRR_VALUE>>8); // BAUDERATE
	UBRRL = (uint8_t)UBRR_VALUE;
	UCSRC |= (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);  //8 bitov, bez parity, 1 stop
	UCSRB |= (1<<RXEN)|(1<<TXEN);	// povolenie rx a tx
}
int USART_send_1byte(char u8data)
{
	while(!(UCSRA& (1<<UDRE))){};
	UDR = u8data;
	return 0;
}
int USART_receive_1byte(void)
{
	uint8_t u8data;
	while(!(UCSRA& (1<<RXC))){};
	u8data = UDR;
	return u8data;
}
void ADC_1(uint8_t kanal)  //kanal v rozsahu 0 az 32 vid tabulka
{
	kanal &= 0b00011111; //maskovane
	ADMUX = 0b01100000|kanal;  //referencia AVcc, ADLAR- vysledok 8bit v ADH
	// staci citat ADCH, kanal 0, ref= AVcc
	ADCSRA = 0b10000110; // povolenie ADC, predelic hodin 64, jednoraz. p
}
uint8_t prevod()
{
	ADCSRA |=(1<<ADSC); //start prevodu
	while((ADCSRA & (1<<ADIF)) == 0){};
	return(ADCH);
}
/*Button
if (is_low(PB2/3)
{
	if (is_high(PB2/3))
	{
		príkaz
	}
}
*/
#endif /* INCFILE1_H_ */
