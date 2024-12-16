#pragma once

# include "terminal/_data.h"
# include "terminal/vga.h"
# include "terminal/tty.h"



uint8_t	putChar(char c);
uint8_t	putCharPos(char c, uint32_t x, uint32_t y);

void	setCursor(uint8_t x, uint8_t y);

void	incrementPositionX(_tty *tty);
void	incrementPositionY(_tty *tty);
void	decrementPositionY(_tty *tty);
