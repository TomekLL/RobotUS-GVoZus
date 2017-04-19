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
	//DDRA = 255;
	//PORTA = 255;
	
	speed=100;
	OCR0= CAS10;
	TCCR0=REZIM;
	//TIMSK |=(1<<OCIE0);			// lokalne povolenie prerusenia
	//zapnutie USART spojenia
	setUSART();
	//aktivacia motorov
	nastav();
	//globalne povolenie prerusenia
	sei();
	while (1) 
	{
		speedL = 0;
		speedR = 0;
		hodnota = USART_receive_1byte();
		
		if(hodnota & 64) {
			speedL+=speed;
		}
		if(hodnota & 32) {
			speedL+=speed;
		}
		if(hodnota & 16) {
			speedL+=speed;
		}
		if(hodnota & 8) {
			speedL+=speed;
			speedR+=speed;
			sbi(PORTA, PA6);
			sbi(PORTA, PA5);
		} else {
			cbi(PORTA, PA5);
			cbi(PORTA, PA6);
		}
		if((hodnota & 4) == (unsigned char) 4) {
			speedR+=speed;
		}
		if((hodnota & 2) == (unsigned char) 2) {
			speedR+=speed;
		}
		if((hodnota & 1) == (unsigned char) 1) {
			speedR+=speed;
		}
		go(0 , speedR);
		go(1 , speedL);
	}
}
// 	prerusenie volane cca kazdych 30ms
ISR(TIMER0_COMP_vect)
{
	//zobratie hodnoty z USART spojenia
	hodnota = USART_receive_1byte();	
	distance = read_sonar();
}

