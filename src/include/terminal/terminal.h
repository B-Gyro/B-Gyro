#pragma once

# include "terminal/vga.h"
# include "terminal/tty.h"

# define MAX_HISTORY	20
# define MAX_LINE		127

# define MAX_TTYS		3


typedef struct terminal {
	// after malloc change 3 to MAX_TTYS
	_tty	ttys[3];
	_tty	*currentTTY;
} _terminal;

extern _terminal g_terminal;

uint8_t putChar(char c);
uint8_t putCharPos(char c, uint32_t x, uint32_t y);
void	putCellOnVga(_vgaCell cell, uint8_t x, uint8_t y);
void	setCursor(uint8_t x, uint8_t y);
// void	bzero(uint16_t *address, uint32_t size);

void updatePositionX(_tty *tty);

void bigBzero(uint16_t *address, uint32_t size);
