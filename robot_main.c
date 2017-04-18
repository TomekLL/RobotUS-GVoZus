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

#include "init.h"
//kniznica obsahujuca USART spojenie
#include "USART.h"
#include "motory.h"

int main(void)
{
	//zapnutie USART spojenia
	setUSART();
	//nastavenie vystupov na A (zapnutie vystupu pre ledky);
	DDRA |= (1 << PA5) | (1 << PA6) | (1<< PA7);
	//vypnutie lediek (nastavenie na 1 z dovodu invertovaneho systemu lediek)
	sbi(PORTA,PA5);
	sbi(PORTA,PA6);
	sbi(PORTA,PA7);
	//nekonecny cyklus
	while (1) 
	{
		//init hodnoty value
		unsigned char value = 0;
		//zobratie hodnoty z USART spojenia
		value = USART_receive_1byte();
		if(value == (unsigned char)8) {
			//8 = 0b00001000 (stredovy senzor je aktivovany)
			//zapnutie lavej led 
			cbi(PORTA,PA5);
			//vypnutie pravej led a bzuciaku
			sbi(PORTA,PA6);
			sbi(PORTA,PA7);
		} else if(value == (unsigned char)127) {
			//127 = 0b01111111 (vsetky senzory su aktivne)
			//zapnutie oboch led
			cbi(PORTA,PA5);
			cbi(PORTA,PA6);
			//vypnutie bzuciaku
			sbi(PORTA,PA7);
		} else if(value == (unsigned char)0) {
			//0 = 0b00000000 (ani jeden senzor nie je aktivny)
			sbi(PORTA,PA6);
			sbi(PORTA,PA5);
			cbi(PORTA,PA7);
		} else {
			//pre pripad ze dostavame iny vysledok (ciara sa nachcadza na inom nez strednom senzore)
			//vypnutie vsetkeho
			sbi(PORTA,PA5);
			sbi(PORTA,PA6);
			sbi(PORTA,PA7);
		}
	}
}

