/*
 * yrobot_plosak.c
 *
 * Created: 13.04.2017 16:20:27
 * Author : filip
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include "kniznica.h"


int main(void) {
    
	setUSART();
	DDRA = 255;
	PORTA = 255;

	while(1) {
		USART_send_1byte((unsigned char) 1);	
		_delay_ms(100);
		USART_send_1byte((unsigned char) 0);
		_delay_ms(100);
	}	
}

