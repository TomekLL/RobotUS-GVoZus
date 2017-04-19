#include "init.h"
//kniznica vyuzita na USART komunikaciu
#include "USART.h"

//definovanie hranicnej hodnoty voltov (nie je x(hodnota) V ale pomer ku 5 voltom) 
uint16_t LINE_VOLTAGE_MIN;
//hodnota pozadia
uint16_t voltageBackground;
//hodnota popredia
uint16_t voltageForeground;
//urcuje ci urcovaci cyklus prebehol(urcovanie voltage)
bool voltageSet;
//tenBit
uint16_t tenBit;
//hodnota ktoru skenujeme
uint16_t checkingValue;
//hodnota ktoru ideme odoslat
unsigned char sendingValue;
//ratac cipov
uint8_t counter;
//zakladna hodnota admux
uint8_t admux_base;
//osvietenie
bool svieti;

void sendValues();
uint8_t getBool(uint16_t v);
uint16_t getCheckingValue();
