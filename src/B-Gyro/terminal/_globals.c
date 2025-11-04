#include "terminal/terminal.h"
#include "terminal/vga.h"

// TERMINAL ************************************************************/

_terminal	g_terminal;
_list		g_users;
uint32_t	g_id = 0;


// inchaellah all this will be gone after handling allocation
// _node g_rows[MAX_TTYS][_MAX_ROWS];

// _node g_commandLine[MAX_TTYS][DEFAULT_MAX_KEYBOARD_BUFFER];

// _node g_usersNodes[MAX_USERS];

// _list g_buffers[MAX_TTYS];
// _list g_histories[MAX_TTYS];

// _vgaCell	g_ttyBuffers[MAX_TTYS][_MAX_ROWS][_MAX_COLUMNS];
// uint8_t		g_historyBuffers[MAX_TTYS][MAX_HISTORY][DEFAULT_MAX_KEYBOARD_BUFFER];
// _user		g_usersData[MAX_USERS] = {0};

// VGA ************************************************************/

// sorted by: index == vga code
const int g_ansi[16] = {30, 34, 32, 36, 31, 35, 33, 37, 90, 94, 92, 96, 91, 95, 93, 97};

uint8_t g_currentTextColor = DEFAULT_TEXT_COLOR;
uint8_t g_currentBackGroundColor = DEFAULT_BACKGROUND_COLOR;