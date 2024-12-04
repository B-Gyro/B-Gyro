# pragma once

# include "klibc/types.h"

uint8_t		pByteIn(uint16_t portNumber);
void		pByteOut(uint16_t portNumber, uint8_t value);
uint16_t	pWordIn(uint16_t portNumber);
void		pWordOut(uint16_t portNumber, uint16_t value);
void		ioWait(void);