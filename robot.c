/*
 * yrobot_robot.c
 *
 * Created: 13.04.2017 16:22:01
 * Author : filip
 */ 

#include <avr/io.h>
#include "kniznica.h"


int main(void)
{
	setUSART
	DDRA |= (1 << PA5) | (1 << PA6);
    while (1) 
    {
		unsigned char value = 0;
		value = USART_receive_1byte();
		if(value == (unsigned char) 1) { 
			cbi(PORTA,PA6);
			cbi(PORTA,PA5);
		} else { 
			sbi(PORTA,PA6);
			sbi(PORTA,PA5);
		}
    }
}

