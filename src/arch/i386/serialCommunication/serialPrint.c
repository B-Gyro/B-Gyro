#include "arch/i386/ports/portsIO.h"
#include "arch/i386/ports/serialCommunication/serialIO.h"
#include "klibc/print.h"

extern _bGyroStats g_bGyroStats;

// checks if serial communication ready for data transmition
int isTransmitEmpty() {
	return portByteIn(COM1 + 5) & 0x20;
}

// putchar for serial Communication
uint8_t serialPutChar(char c){
	if (!(g_bGyroStats.hasSerialWorking))
		return 0;
	while (isTransmitEmpty() == 0);
	portByteOut(COM1, c);
	return 1;
}