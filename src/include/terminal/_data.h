#pragma once

// ******************** LIBRARIES **************************************
#include "klibc/types.h"
#include "klibc/print.h"
#include "drivers/keyboard.h"

// ******************** MACROS **************************************

#define DEFAULT_TEXT_COLOR 0X07
#define DEFAULT_BACKGROUND_COLOR 0x00

#define MAX_TTYS 3
#define TAB_SIZE 4

// max commandlines to be stored in history
#define MAX_HISTORY 20

#define MAX_ROWS 24 // 25 last line reserved for status
#define MAX_COLUMNS 80

// MAX_COLUMNS * MAX_COLUMNS must be divisible by 32
// 16 * MAX_COLUMNS must be divisible by 32
#define SCREEN_SIZE MAX_COLUMNS *MAX_COLUMNS
#define FULL_SCREEN_SIZE MAX_COLUMNS *(MAX_COLUMNS + 1)

// ******************** TYPEDEFS **************************************

typedef struct vgaCell
{
	char character;
	char color;
} _vgaCell;

// ***** Doubly linked list *******

typedef struct node
{
	// _vgaCell buffer[MAX_COLUMNS];
	void		*ptr;
	struct node *previous;
	struct node *next;
} _node;

typedef struct list
{
	void *first;
	void *last;
	void *current;
	uint32_t size;
} _list;

// ********************************

typedef struct tty
{
	_list *buffer;
	_list *history;

	uint8_t index;

	uint32_t posX;
	uint32_t posY;

	uint8_t textColor;
	uint8_t backgroundColor;

	_kbdBuffer keyboardBuffer;

	_vgaCell status[MAX_COLUMNS];
} _tty;

typedef struct terminal
{
	_tty ttys[MAX_TTYS];
	_tty *currentTTY;
} _terminal;

// ******************** GLOBALS **************************************

extern _terminal g_terminal;

extern uint8_t g_currentTextColor;
extern uint8_t g_currentBackGroundColor;
extern const int g_ansi[16];

// -----------------
extern _list g_buffers[MAX_TTYS];
extern _list g_histories[MAX_TTYS];

extern _node g_rows[MAX_TTYS][MAX_ROWS];
extern _node g_commandLine[MAX_TTYS][MAX_KEYBOARD_BUFFER];

extern _vgaCell g_ttyBuffers[MAX_TTYS][MAX_ROWS][MAX_COLUMNS];
extern unsigned char *g_historyBuffers[MAX_TTYS][MAX_HISTORY][MAX_KEYBOARD_BUFFER];
