#pragma once

// ******************** LIBRARIES **************************************
# include "klibc/types.h"
# include "klibc/print.h"

// ******************** MACROS **************************************


# define DEFAULT_TEXT_COLOR			0X07
# define DEFAULT_BACKGROUND_COLOR	0x00


# define MAX_TTYS	3
# define TAB_SIZE	4

// max commandlines to be stored in history
# define MAX_HISTORY	20 

// max size of commandline [keyboard buffer]
# define MAX_LINE		200

# define MAX_ROWS		24 // 25 last line reserved for status
# define MAX_COLUMNS	80

// MAX_ROWS * MAX_COLUMNS must be divisible by 32
// 16 * MAX_COLUMNS must be divisible by 32
# define SCREEN_SIZE		MAX_COLUMNS * MAX_ROWS
# define FULL_SCREEN_SIZE	MAX_COLUMNS * (MAX_ROWS + 1)

// ******************** TYPEDEFS **************************************

typedef struct vgaCell
{
	char character;
	char color;
} _vgaCell;

// ***** Doubly linked list *******

typedef struct node
{
	_vgaCell buffer[MAX_COLUMNS];
	struct node *previous;
	struct node *next;
} _node;

typedef struct list
{
	_node *first;
	_node *last;
	uint32_t size;
} _list;

// ********************************

typedef struct tty
{
	_list *buffer;

	uint8_t index;

	uint32_t posX;
	uint32_t posY;

	uint8_t textColor;
	uint8_t backgroundColor;

	_vgaCell status[MAX_COLUMNS];
	_vgaCell history[MAX_HISTORY][MAX_LINE];
} _tty;

typedef struct terminal
{
	// after malloc change 3 to MAX_TTYS
	_tty ttys[3];
	_tty *currentTTY;
} _terminal;

// ******************** GLOBALS **************************************

extern _terminal	g_terminal;
extern _node		g_nodes[MAX_TTYS][MAX_ROWS];

extern	uint8_t		g_currentTextColor;
extern	uint8_t		g_currentBackGroundColor;
extern	const int	g_ansi[16];