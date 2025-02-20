#pragma once

// ******************** LIBRARIES **************************************
# include "images/image.h"
// # include "klibc/types.h"
# include "klibc/print.h"
# include "drivers/keyboard.h"
# include "bGyro.h"

// ******************** MACROS **************************************


# define MAX_USERS				4
# define MAX_NAME_LENGTH		20
# define MAX_PASSWORD_LENGTH	20

# define CURRENT_USER		((_user *)g_users.current->ptr)
# define CURRENT_TTY		(g_terminal.currentTTY)
# define CURSOR_X			(g_terminal.currentTTY->cursorX)
# define CURSOR_Y			(g_terminal.currentTTY->cursorY)
# define POSITION_X			(g_terminal.currentTTY->posX)
# define POSITION_Y			(g_terminal.currentTTY->posY)
# define CURSOR_AT_THE_END	((CURRENT_TTY->cursorX == CURRENT_TTY->posX) && (CURRENT_TTY->cursorY == CURRENT_TTY->posY))
# define POS_Y_LAST			((CURRENT_TTY->posY == (MAX_ROWS - 1)))

# define CURSOR_UP		0x48
# define CURSOR_DOWN	0x50
# define CURSOR_RIGHT	0x4D
# define CURSOR_LEFT	0x4B

# define DEFAULT_TEXT_COLOR			0X07
# define DEFAULT_BACKGROUND_COLOR	0x00

# define MAX_TTYS 3
# define TAB_SIZE 4

// max commandlines to be stored in history
# define MAX_HISTORY	20

# define _MAX_ROWS		50
# define _MAX_COLUMNS	100

# define TTY_HEIGHT		CURRENT_TTY->mode->screenHeight
# define TTY_WIDTH		CURRENT_TTY->mode->screenWidth
# define FONT_HEIGHT	CURRENT_TTY->font->height
# define FONT_WIDTH		CURRENT_TTY->font->width

# define MAX_ROWS		(FONT_HEIGHT ? (TTY_HEIGHT / FONT_HEIGHT) - 1 : TTY_HEIGHT) // last line reserved for status
# define MAX_COLUMNS	(FONT_WIDTH ? TTY_WIDTH / FONT_WIDTH : TTY_WIDTH)

// MAX_COLUMNS * MAX_ROWS must be divisible by 32
// 16 * MAX_COLUMNS must be divisible by 32
# define SCREEN_SIZE		0
# define FULL_SCREEN_SIZE	1

# define COLOR_BLACK			"\033[30m"
# define COLOR_BLUE				"\033[34m"
# define COLOR_GREEN			"\033[32m"
# define COLOR_CYAN				"\033[36m"
# define COLOR_RED				"\033[31m"
# define COLOR_MAGENTA			"\033[35m"
# define COLOR_BROWN			"\033[33m"
# define COLOR_LIGHT_GREY		"\033[37m"
# define COLOR_DARK_GREY		"\033[90m"
# define COLOR_LIGHT_BLUE		"\033[94m"
# define COLOR_LIGHT_GREEN		"\033[92m"
# define COLOR_LIGHT_CYAN		"\033[96m"
# define COLOR_LIGHT_RED		"\033[91m"
# define COLOR_LIGHT_MAGENTA	"\033[95m"
# define COLOR_YELLOW			"\033[93m"
# define COLOR_WHITE			"\033[97m"
# define COLOR_DEFAULT			"\033[39m"
# define COLOR_RESET			"\033[0m"

// typedef struct font _font;

// ******************** TYPEDEFS **************************************

typedef struct vgaCell
{
	uint8_t character;
	uint8_t color;
} _vgaCell;

// ***** Doubly linked list *******

typedef struct node
{
	void *ptr;
	struct node *previous;
	struct node *next;
} _node;

typedef struct list
{
	_node *first;
	_node *last;
	_node *current;
	uint32_t size;
} _list;

// ********************************

typedef struct user {
    char username[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
	uint16_t	id;
} _user ;

typedef struct tty
{
	_list *buffer;
	_list *history;

	_kbdBuffer keyboardBuffer;

	uint8_t index;

	uint32_t posX;
	uint32_t posY;

	uint32_t cursorX;
	uint32_t cursorY;

	uint8_t textColor;
	uint8_t backgroundColor;

	_vgaCell status[_MAX_COLUMNS];

	_image	 *font;
	_vgaMode *mode;
} _tty;

typedef struct terminal
{
	_tty		ttys[MAX_TTYS];
	_tty		*currentTTY;
} _terminal;

// ******************** GLOBALS **************************************

extern uint32_t	g_id;

extern _list		g_users;

extern _terminal	g_terminal;
extern _bGyroStats	g_bGyroStats;

extern uint8_t	g_currentTextColor;
extern uint8_t	g_currentBackGroundColor;
extern const int g_ansi[16];

// -----------------
extern _list g_buffers[MAX_TTYS];
extern _list g_histories[MAX_TTYS];

extern _node g_rows[MAX_TTYS][_MAX_ROWS];
extern _node g_commandLine[MAX_TTYS][DEFAULT_MAX_KEYBOARD_BUFFER];
extern _node g_usersNodes[MAX_USERS];

extern _vgaCell g_ttyBuffers[MAX_TTYS][_MAX_ROWS][_MAX_COLUMNS];
extern uint8_t g_historyBuffers[MAX_TTYS][MAX_HISTORY][DEFAULT_MAX_KEYBOARD_BUFFER];
extern _user g_usersData[MAX_USERS];
