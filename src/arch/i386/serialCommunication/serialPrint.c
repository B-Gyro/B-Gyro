#include "arch/i386/ports/portsIO.h"
#include "arch/i386/ports/serialCommunication/serialIO.h"

// checks if serial communication ready for data transmition
int isTransmitEmpty() {
	return portByteIn(COM1 + 5) & 0x20;
}

// putchar for serial Communication
uint8_t serialPutChar(char c){

	(void)c;
	//while (isTransmitEmpty() == 0);
	//portByteOut(COM1, c);
	return 1;
}