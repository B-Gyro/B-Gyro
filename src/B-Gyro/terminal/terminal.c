#include "terminal/terminal.h"
#include "terminal/vga.h"
#include "terminal/tty.h"
#include "klibc/memory.h"

void	initTerminal() {
	g_terminal.currentTTY = g_terminal.ttys;

	for (uint8_t i = 0; i < MAX_TTYS; i++)
		g_terminal.ttys[i].buffer = g_buffers + i;
	
	initTTY(0);
}

void scroll(void)
{
	_list *buffer;

	buffer = g_terminal.currentTTY->buffer;
	buffer->first = ((_node *)buffer->first)->next;
	buffer->last = ((_node *)buffer->last)->next;

	bigBzero(((_node *)buffer->last)->buffer, MAX_COLUMNS);
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
		tty->buffer->last = ((_node *)tty->buffer->last)->previous;
		tty->posX = MAX_COLUMNS - 1;
	}
}

void incrementPositionY(_tty *tty)
{
	if (tty->buffer->size >= MAX_ROWS)
		scroll();
	else
	{
		tty->buffer->size++;
		tty->posY++;
		bigBzero(((_node *)tty->buffer->last)->next->buffer, MAX_COLUMNS);
		tty->buffer->last = ((_node *)tty->buffer->last)->next;
	}
}

void incrementPositionX(_tty *tty)
{
	tty->posX++;
	if (tty->posX >= MAX_COLUMNS)
	{
		tty->posX = 0;
		incrementPositionY(tty);
	}
}

void setCursor(uint8_t x, uint8_t y)
{
	uint32_t pos = y * MAX_COLUMNS + x;

	portByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
	portByteOut(VGA_DATA_REGISTER, (unsigned char)(pos >> 8));

	portByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
	portByteOut(VGA_DATA_REGISTER, (unsigned char)(pos & 0xff));
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
}