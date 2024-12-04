#include "arch/i386/ports/portsIO.h"
#include "arch/i386/ports/serialCommunication/serialIO.h"

// checks if serial communication ready for data transmition
int isTransmitEmpty() {
	return pByteIn(COM1 + 5) & 0x20;
}

// putchar for serial Communication
uint8_t serialPutCharacter(char c){
	while (isTransmitEmpty() == 0);
	pByteOut(COM1, c);

	return 1;
}