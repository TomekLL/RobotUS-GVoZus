#ifndef init_H_
#define init_H_

#ifndef F_CPU
#define F_CPU	10000000UL	//definicia frekvencie CPU
#endif

#define BAUDERATE 9600
#define UBRR_VALUE ((F_CPU / (BAUDERATE * 16UL)) -1)  //prenosova rychlost

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#endif /* init_H_ */
