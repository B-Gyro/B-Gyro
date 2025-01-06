#pragma once

# include "terminal/_data.h"
# include "drivers/keyboard.h"

// tty
void	initTTY(uint8_t index);
void	switchTTY(uint8_t index);
void	clearTTY(uint32_t size);

// buffer
void	putTtyBuffer(void);

// history
void	initHistory(void);
void	addToHistory(void);
void	printHistory(void);
void	getHistory(uint8_t cursor);

// status bar
void	updateStatusBar(void);
