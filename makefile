all: motory plosak main sonar usart
	avr-gcc -Wall -Os -DF_CPU=1000000 -mmcu=atmega16 -o code.elf motory.o plosak_main.o robot_main.o sonar.o usart.o
	rm -f code.hex
	avr-objcopy -j .text -j .data -O ihex code.elf code.hex
	rm -f code.elf

motory:
	avr-gcc -Wall -Os -DF_CPU=1000000 -mmcu=atmega16 -c motory.c -o motory.o

plosak:
	avr-gcc -Wall -Os -DF_CPU=1000000 -mmcu=atmega16 -c plosak_main.c -o plosak_main.o

main:
	avr-gcc -Wall -Os -DF_CPU=1000000 -mmcu=atmega16 -c robot_main.c -o robot_main.o

sonar:
	avr-gcc -Wall -Os -DF_CPU=1000000 -mmcu=atmega16 -c sonar.c -o sonar.o

usart:
	avr-gcc -Wall -Os -DF_CPU=1000000 -mmcu=atmega16 -c USART.c -o usart.o