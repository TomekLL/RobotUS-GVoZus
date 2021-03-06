/* NAZOV
 * yrobot_plosak.c
 *
 * HARDWARE NAROKY
 * - farebne senzory pripojene na porty PA0 - PA7
 * - podsvietenie napojene na PORTB pin 4
 * - USART spojenie s druhou atmegou
 * 
 * FUNKCIA
 * - hodnota zo senzorov (vo voltoch) je prememena na cislo v binarnej sustave (priblizne pomer voltov zo senzorov ku 5V)
 * - pri prvom prebehnuti senzorov sa nastavy hodnota ktora ukazuje hranicu medzi pozadim a ciarov
 * - dve tieto hodnoty (jednu pre meranie s podsvietenim a jedna bez) su nasledne zpriemerovane a pripravene na rozhodovanie
 * - cip rozhoduje ci je tato hodnota(zpriemerovana) ciara alebo nie (pomocou funkcie getBool(uint16_t v))
 * - cip po zozbierani vsetkych hodnot vysle informaciu vo forme char (8-bitova informacia) cez USART spojenie
 * ! informacia je sice prenasana cez 8 bitove spojenie no naozaj ma len 7 bitov (kvoli 7 senzorom)
 * - bity v tomto chare (0 - 7 bit) predstavuju nachadzaju sa (1) alebo nenachadzaju sa (0) ciaru
 * - n-ty bit v informacii predstavuje informaciu z n-teho senzoru (1 - ciara, 0 - pozadie)
 *
 * Created: 13.04.2017 16:20:27
 * Author : Filip
 */ 

#include "plosak.h"

int main(void) {
	// zapnutie USART spojenia
	setUSART();
	// zapnutie kalibrovania
	voltageSet = 0;
	// zakldane nastavenie hranicnej hodnoty (zaloha)
	LINE_VOLTAGE_MIN = (uint16_t) 400;

	// pociatocna svetelna sekvencia na overenie USART portu
	_delay_ms(25);
	USART_send_1byte((unsigned char) 8);
	_delay_ms(25);
	USART_send_1byte((unsigned char) 4);
	_delay_ms(25);
	USART_send_1byte((unsigned char) 16);
	_delay_ms(25);

	// nastavovanie delenia (nastavene pre 128)
	ADCSRA |= 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0;
	// nastavovanie zakladnej hodnoty admux  (bez toho aby sme poukazovali na port vstupu)
	ADMUX  |= 1<<REFS0;
	// ukladanie zakladnej hodnoty
	admux_base = ADMUX;
	// urcovanie prijmacieho portu pre ADC
	counter = 1;
	ADMUX  |= 1<<MUX1;
	// povolovanie ADC
	ADCSRA |= 1<<ADEN; 
	// zapinanie prerusovania pre ADC
	ADCSRA |= 1<<ADIE;
	// povolenie preruseni 
	sei();
	// zapnutie convertovania 
	ADCSRA |= 1<<ADSC;

	// pripravenie DDRB pre podsvecovanie
	DDRB  |= 1<<PB4;
	// vypnutie lampy
	svieti = 0;
	PORTB |= 1<<PB4; // funguje invertovane
	// prazdny nekonecny cyklus
	while(1) {
	}
	
}
/*
*	prerusenie zavinene prijmom z ADC
*   - stara sa o zbieranie informacii zo senzorov
*/
ISR(ADC_vect) {

	//ukladanie low hodnoty (spodne bity)
	uint8_t low = ADCL;
	//ukladanie celej hodnoty (hodnoty spodnych bitov spojene s hornymi)
	uint16_t tenBit = ADCH << 8 | low;

	//vyhodnocovanie farby
	if(svieti) {
		if(voltageSet) {
			//zapisanie hodnoty
			value|=(getBool((uint16_t)((tenBit + checkingValue) / 2))<<(counter-1));
		} else {
			//nastavovanie zakladnej hodnoty
			if(counter == 1) {
				//ziskavanie hodnoty pozadia z prveho cidla
				voltageBackground = ((tenBit + checkingValue) / 2); 
			} else if(counter == 4) {
				//ziskavanie hodnoty ciary z stredneho cidla (cidlo cislo 4)
				voltageForeground = ((tenBit + checkingValue) / 2);
				LINE_VOLTAGE_MIN = (uint16_t)(voltageForeground);
				voltageSet = 1;
			}
		}
		//prechod na dalsii senzor
		counter++;
		if(counter>7) {
			//v pripade ze sme precitali vsetky senzory
			counter = 1;
			USART_send_1byte((unsigned char) value);
			value &= 0;
			voltageSet = 1;
		}
		//nastavenie skenovanie na n-ty port kde n predstavuje counter
		ADMUX = admux_base | counter;
		//zhasnutie svetiel
		PORTB|= (1<<PB4);
		svieti = 0;
	} else {
		//nastavenie prechodnej hodnoty (temp)
		checkingValue = 0; //nemyslim si ze priemerovanie hodnot je dobry napad len sa stracia presnost
		//zapnutie svetiel
		PORTB&= ~(1<<PB4);
		svieti = 1;
	}
	//opatovne zacanie ADC ratania
	ADCSRA |= 1 << ADSC;
}

/*
*	rozhodovanie ci je hodnota zoskenovana cidlami ciara alebo pozadie
*	porovnava s hodnotou ziskanou prvotnym meranim
*	@param v hodnota zo senzora (jedneho, nie binarne zhrnutie senzorov)
*/
uint8_t getBool(uint16_t v) {
	return v >= LINE_VOLTAGE_MIN;
}

