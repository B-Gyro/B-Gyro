#pragma once

# include "terminal/_data.h"
# include "drivers/keyboard.h"

extern bool g_shellMode;

// tty
void	initTTY(uint8_t index);
void	switchTTY(uint8_t index);
void	clearTTY(bool fullScreen);

// buffer
void	putTtyBuffer(void);
void	putPartOfBuffer(uint32_t cursorX);

// history
void	initHistory(void);
void	addToHistory(void);
void	printHistory(void);
void	getHistory(uint8_t cursor);

// status bar
void	updateStatusBar(void);
