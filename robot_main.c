/* NAZOV
 * yrobot_robot.c
 *
 * FUNKCIA
 * - nadviazanie USART spojenia so sparovanou ATMEGOU
 * - sustavne preberanie hodnoty zo sparovanej ATMEGY
 * - reagovanie na hodnoty pomocu LED a BZUCIAKU
 *
 * Created: 13.04.2017 16:22:01
 * Author : Filip
 */ 

#include "robot.h"

int main(void)
{
	speed=100;
	OCR0= CAS10;
	TCCR0=REZIM;
	TIMSK |=(1<<OCIE0);			// lokalne povolenie prerusenia
	//zapnutie USART spojenia
	setUSART();
	//aktivacia motorov
	nastav();
	//globalne povolenie prerusenia
	sei();
	while (1) 
	{
		if(value == (unsigned char)8) 
		{
			//8 = 0b00001000 (stredovy senzor je aktivovany)
			go(0,speed);	//pravy
			go(1,speed); //lavy
		} 
		else if(value == (unsigned char)4)
			{
				//4 = 0b00000100
				go(0,speed-15);
				go(1,speed);
			}
			else if(value == (unsigned char)16) 
				{
					//16 = 0b00010000
					go(0,speed);
					go(1,speed-15);
				}
	}
}
// 	prerusenie volane cca kazdych 30ms
ISR(TIMER0_COMP_vect)
{
	//init hodnoty value
	unsigned char value = 0;
	unsigned char distance = 0;
	//zobratie hodnoty z USART spojenia
	value = USART_receive_1byte();	
	distance = read_sonar();
}
