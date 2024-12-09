# pragma once

# include "klibc/types.h"

uint8_t		portByteIn(uint16_t portNumber);
void		portByteOut(uint16_t portNumber, uint8_t value);
uint16_t	portWordIn(uint16_t portNumber);
void		portWordOut(uint16_t portNumber, uint16_t value);
void		ioWait(void);