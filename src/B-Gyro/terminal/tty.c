#include "terminal/_data.h"
#include "terminal/vga.h"
#include "terminal/tty.h"
#include "terminal/terminal.h"
#include "klibc/memory.h"
#include "sshell/sshell.h"
#include "bGyro.h"

extern _bGyroStats g_bGyroStats;

void initTTY(uint8_t index) {
	_tty	*tty = g_terminal.currentTTY;
	_node	*ptr;
	uint8_t	i;

	tty->posX = 0;
	tty->posY = 0;

	tty->index = index;

	tty->textColor = DEFAULT_TEXT_COLOR;
	tty->backgroundColor = DEFAULT_BACKGROUND_COLOR;

	tty->buffer->first = &g_rows[index][0];
	ptr = tty->buffer->first;
	for (i = 0; i < (MAX_ROWS - 1); i++) {
		ptr->ptr = &g_ttyBuffers[index][i];
		ptr->next = &g_rows[index][i + 1];
		if (i)
			ptr->previous = &g_rows[index][i - 1];
		ptr = ptr->next;
	}
	ptr->ptr = &g_ttyBuffers[index][i];
	ptr->next = tty->buffer->first;
	tty->buffer->first->previous = &g_rows[index][MAX_ROWS - 1];

	tty->buffer->last = tty->buffer->first;

	g_terminal.currentTTY->buffer->size = 1;
	initHistory();
	updateStatusBar();

	SERIAL_SUCC("TTY %d Initialized", index);
}

void clearTTY(uint32_t size)
{
	clearVGA(size);

	g_terminal.currentTTY->buffer->size = 1;
	bigBzero(g_terminal.currentTTY->buffer->first->ptr, MAX_COLUMNS);
	g_terminal.currentTTY->buffer->last = g_terminal.currentTTY->buffer->first;

	if (size == FULL_SCREEN_SIZE)
		bigBzero(g_terminal.currentTTY->status, MAX_COLUMNS);
}

void putTtyBuffer(void)
{
	_tty		*tty;
	_node		*line;

	tty = g_terminal.currentTTY;

	line = tty->buffer->first;

	clearVGA(SCREEN_SIZE);

	for (tty->posY = 0; tty->posY < g_terminal.currentTTY->buffer->size; tty->posY++)
	{
		for (tty->posX = 0; tty->posX < MAX_COLUMNS; tty->posX++)
		{
			if (((_vgaCell *)line->ptr)[tty->posX].character == '\0' ||
				((_vgaCell *)line->ptr)[tty->posX].character == '\n')
				break;
			putCellOnVga(((_vgaCell *)line->ptr)[tty->posX], tty->posX, tty->posY);
		}
		line = line->next;
	}
	tty->posY--;
	if (tty->posX >= (MAX_COLUMNS - 1)) {
		incrementPositionX(tty);
		putTtyBuffer();
	}
	putCharPos(' ', tty->posX, tty->posY);
	setCursor(tty->posX, tty->posY);
}

void switchTTY(uint8_t index)
{
	_tty *tty;

	if ((index >= MAX_TTYS) || (index == g_terminal.currentTTY->index))
		return;

	g_terminal.currentTTY->textColor = g_currentTextColor;
	g_terminal.currentTTY->backgroundColor = g_currentBackGroundColor;

	tty = &(g_terminal.ttys[index]);
	g_terminal.currentTTY = &(g_terminal.ttys[index]);

	keyboardSetBuffer(&(g_terminal.ttys[index].keyboardBuffer), 0);

	if (tty->index != index) {
		initTTY(index);
		interruptPrompting();
	}
	else
		updateStatusBar();

	g_currentTextColor = tty->textColor;
	g_currentBackGroundColor = tty->backgroundColor;
	putTtyBuffer();
}

/*------------------------------ STATUS BAR ------------------------------*/

void	clearStatusBar(void) {
	// 1920 = 24 * 80
	bigBzero((uint16_t *)VIDEO_ADDRESS + 24*80, MAX_COLUMNS);
	bigBzero(g_terminal.currentTTY->status, MAX_COLUMNS);
}

void	updateStatusBar(void) {
	char content[80];

	clearStatusBar();
	SPRINTF(content, "TTY: %d | OSVersion: "COLOR_LIGHT_CYAN"%s"COLOR_DEFAULT" | STATE: %s |", 
		g_terminal.currentTTY->index + 1,
		g_bGyroStats.OSVersion,
		bGyroStatusToString(g_bGyroStats.status));
	putStrPos(content, 0, MAX_ROWS);
}

/*------------------------------------------------------------------------*/
