#define A_REZIM	0xa1;
#define B_REZIM	0xa;

#define sbi(x,y) x |= _BV(y)	//set bit- nastavenie bitu
#define cbi(x,y) x &= (~(_BV(y)))	//clear bit - nulovanie bitu
#define tbi(x,y) x ^=_BV(y)		//toggle bit - zmena bitu

void nastav();
void go(uint8_t motor, int rychlost);  // MOTOR 0=A, 1=B rychlost -255 az 255
