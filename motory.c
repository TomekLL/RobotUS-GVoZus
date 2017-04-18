#include "init.h"
#include "motory.h"

void nastav()
{
	DDRD |=(1<<PD4)|(1<<PD5); 
	/*motory, USART  PD6= SMER_A, PD7= SMER_B,
	PD4=RYCHLOST_B, PD5=RYCHLOST_A  */ 
	DDRB |=(1<<PB4);	//rezim motorov vystup
	TCCR1A = A_REZIM;
	TCCR1B = B_REZIM;
	OCR1AH = 0;			//MOTOR_A stoji
	OCR1AL = 0;
	OCR1BH = 0;			//MOTOR_B stoji
	OCR1BL = 0;
	PORTD &= ~(1<<PD6);	//nuluj PD6 SMER_A dopredu
	PORTD &= ~(1<<PD7);	//nuluj PD7 SMER_B dopredu
	PORTB &= ~(1<<PB4);	//nuluj PB4 REZIM s brzdenim
	MCUCR |=(1<<ISC00);	// generuje prerusenie na zmenu
	GICR |=(1<<INT0);	// lokalne povolenie INT0
	GICR |=(1<<INT1); // lokalne povolenie INT1
}
void go(uint8_t motor, int rychlost)
{
	uint8_t m = motor;
	uint8_t r = rychlost;
	if(m==0)		// motor A
	{					
			PORTD |=(1<<PD6);	//nuluj PD6 SMER_A dopredu
			if(r>255)r = 255;
			OCR1A = r;			//MOTOR_A
	}
	if(m==1)		// motor B
	{
		r=r+25;	
		PORTD |=(1<<PD7);	//nuluj PD7 SMER_B dopredu
		if(r>255)r = 255;
		OCR1B=r;			//MOTOR_B
	}
}
