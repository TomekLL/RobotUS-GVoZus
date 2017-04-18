#include "init.h"
#include "USART.h"

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
