# pragma once

# include "klibc/types.h"

uint8_t		portByteIn(uint16_t portNumber);
void		portByteOut(uint16_t portNumber, uint8_t value);
uint16_t	portWordIn(uint16_t portNumber);
void		portWordOut(uint16_t portNumber, uint16_t value);
uint32_t	portDWordIn(uint16_t portNumber);
void		portDWordOut(uint16_t portNumber, uint32_t value);

void		ioWait(void);