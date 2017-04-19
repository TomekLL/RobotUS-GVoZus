all: motory plosak main sonar usart
	avr-gcc -Wall -Os -DF_CPU=1000000 -mmcu=atmega16 -o robot.elf motory.o robot_main.o sonar.o usart.o
	rm -f robot.hex
	avr-objcopy -j .text -j .data -O ihex robot.elf robot.hex
	rm -f robot.elf
	avr-gcc -Wall -Os -DF_CPU=1000000 -mmcu=atmega16 -o plosak.elf plosak_main.o usart.o
	rm -f plosak.hex
	avr-objcopy -j .text -j .data -O ihex plosak.elf plosak.hex
	rm -f plosak.elf

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