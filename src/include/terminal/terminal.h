#pragma once

# include "terminal/_data.h"

typedef struct tty _tty; 

void	initTerminal(void);

uint8_t	putChar(char c);
uint8_t	putCharPos(char c, uint32_t x, uint32_t y);
uint8_t putStrPos(char *str, uint32_t x, uint32_t y);

void	incrementPositionX(void);
void	incrementPositionY(void);
void	decrementPositionX(void);
void	decrementPositionY(void);

void	scroll(void);