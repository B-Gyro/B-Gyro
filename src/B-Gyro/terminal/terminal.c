#include "terminal/terminal.h"
#include "terminal/vga.h"
#include "terminal/tty.h"
#include "klibc/memory.h"

void initTerminal()
{
	g_terminal.currentTTY = g_terminal.ttys;

	// -_-
	for (uint8_t i = 0; i < MAX_TTYS; i++)
	{
		g_terminal.ttys[i].buffer = g_buffers + i;
		g_terminal.ttys[i].history = g_histories + i;
	}

	initTTY(0);
	SERIAL_SUCC("Terminal Initialized");
}

void scroll(void)
{
	_list *buffer;

	buffer = g_terminal.currentTTY->buffer;
	buffer->first = buffer->first->next;
	buffer->last = buffer->last->next;
	buffer->current = buffer->current->next;

	bigBzero(buffer->last->ptr, MAX_COLUMNS);
	putTtyBuffer();
}

void decrementPositionY(_tty *tty)
{
	if (!tty->posY)
		return;
	else
	{
		tty->posY--;
		tty->buffer->size--;
		tty->buffer->last = tty->buffer->last->previous;
		tty->posX = MAX_COLUMNS - 1;
	}
}

void incrementPositionY(_tty *tty)
{
	if (tty->buffer->size >= MAX_ROWS) {
		if (tty->cursorY != tty->posY || \
			tty->cursorX != tty->posX)
			decrementCursorY(tty);
		scroll();
	}
	else {
		tty->buffer->size++;
		tty->posY++;
		bigBzero(tty->buffer->last->next->ptr, MAX_COLUMNS);
		tty->buffer->last = tty->buffer->last->next;
	}
}

void decrementPositionX(_tty *tty)
{
	if (!tty->posX)
	{
		if (!tty->posY)
			return;
		tty->posX = MAX_COLUMNS - 1;
		decrementPositionY(tty);
	}
	else
		tty->posX--;
}

void incrementPositionX(_tty *tty)
{
	tty->posX++;
	// incrementCursorX(tty);
	if (tty->posX >= MAX_COLUMNS)
	{
		tty->posX = 0;
		incrementPositionY(tty);
	}
}

void putCellOnVga(_vgaCell cell, uint8_t x, uint8_t y)
{
	uint32_t pos;
	_vgaCell *adress = (_vgaCell *)VIDEO_ADDRESS;

	pos = y * MAX_COLUMNS + x;
	adress[pos] = cell;
}

void clearVGA(uint32_t size)
{

	bigBzero((uint16_t *)VIDEO_ADDRESS, size);

	g_terminal.currentTTY->posX = 0;
	g_terminal.currentTTY->posY = 0;
	// g_terminal.currentTTY->cursorX = 0;
	// g_terminal.currentTTY->cursorY = 0;
}