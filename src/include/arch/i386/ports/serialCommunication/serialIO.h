# pragma once
# include "klibc/types.h"

#define COM1 0x3F8

int	initSerial();
int	isTransmitEmpty();

uint8_t serialPutChar(char c);