#pragma once

# include "terminal/vga.h"
# include "terminal/terminal.h"

# define MAX_HISTORY	20
# define MAX_LINE	127

# define MAX_ROWS	24	// 25 last line reserved for status
# define MAX_COLUMNS 80

// MAX_ROWS * MAX_COLUMNS must be divisible by 32
// 16 * MAX_COLUMNS must be divisible by 32

# define SCREEN_SIZE MAX_COLUMNS * MAX_ROWS
# define FULL_SCREEN_SIZE MAX_COLUMNS * (MAX_ROWS + 1)

typedef struct node
{
	_vgaCell buffer[MAX_COLUMNS];
	struct node *next;
} _node;

typedef struct list
{
	_node	*first;
	_node	*last;
} _list;

typedef struct tty
{
	_list	*buffer;


	uint8_t index;

	uint32_t posX;
	uint32_t posY;
	uint32_t bufferSize;

	_vgaCell status[MAX_COLUMNS];
	_vgaCell history[MAX_HISTORY][MAX_LINE];
} _tty;

void initTTY(void);
void clearTTY(uint32_t size);
void putTtyBuffer(void);
