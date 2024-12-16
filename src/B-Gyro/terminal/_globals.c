#include "terminal/terminal.h"
#include "terminal/vga.h"

// TERMINAL ************************************************************/

_terminal	g_terminal;

_node	g_nodes[MAX_TTYS][MAX_ROWS];

_list	g_buffers[MAX_TTYS];

char	g_keyboardBuffers[MAX_TTYS][MAX_LINE];



// VGA ************************************************************/

// sorted by: index == vga code
const int	g_ansi[16] = { 30, 34, 32, 36, 31, 35, 33, 37, 90, 94, 92, 96, 91, 95, 93, 97 };

uint8_t	g_currentTextColor = DEFAULT_TEXT_COLOR;
uint8_t	g_currentBackGroundColor = DEFAULT_BACKGROUND_COLOR;