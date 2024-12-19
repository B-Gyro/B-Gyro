#pragma once

# include "terminal/_data.h"
# include "drivers/keyboard.h"

# define COLOR_BLACK        "\033[30m"
# define COLOR_BLUE         "\033[34m"
# define COLOR_GREEN        "\033[32m"
# define COLOR_CYAN         "\033[36m"
# define COLOR_RED          "\033[31m"
# define COLOR_MAGENTA      "\033[35m"
# define COLOR_BROWN        "\033[33m"
# define COLOR_LIGHT_GREY   "\033[37m"
# define COLOR_DARK_GREY    "\033[90m"
# define COLOR_LIGHT_BLUE   "\033[94m"
# define COLOR_LIGHT_GREEN  "\033[92m"
# define COLOR_LIGHT_CYAN   "\033[96m"
# define COLOR_LIGHT_RED    "\033[91m"
# define COLOR_LIGHT_MAGENTA "\033[95m"
# define COLOR_YELLOW       "\033[93m"
# define COLOR_WHITE        "\033[97m"
# define COLOR_DEFAULT      "\033[39m"

// tty
void	initTTY(uint8_t index);
void	switchTTY(uint8_t index);
void	clearTTY(uint32_t size);

// buffer
void	putTtyBuffer(void);

// history
void	initHistory( void );
void	addToHistory( void );
void	printHistory( void );
void	getHistory(uint8_t cursor);

// status bar
void	updateStatusBar(void);