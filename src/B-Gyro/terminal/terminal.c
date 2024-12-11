#include "terminal/terminal.h"
#include "terminal/vga.h"
#include "klibc/memory.h"

// GLOBALS ************************************************************/

_terminal g_terminal = {
	.ttys = {{0}, {0}, {0}},
	.currentTTY = &(g_terminal.ttys[0])};

/**********************************************************************/

// void	initTerminal( void ) {

// }

void putCellOnVga(_vgaCell cell, uint8_t x, uint8_t y)
{
	uint32_t pos;
	_vgaCell *adress = (_vgaCell *)VIDEO_ADDRESS;

	pos = y * MAX_COLUMNS + x;
	adress[pos] = cell;
}

uint8_t putCharPos(char c, uint32_t x, uint32_t y)
{
	_vgaCell cell;

	if (x >= MAX_COLUMNS || y > MAX_ROWS)
		return (0);

	cell.character = c;

	// for now
	cell.color = 0x07;

	putCellOnVga(cell, x, y);
	return (1);
}

void scroll(void)
{
	_list *buffer;

	buffer = g_terminal.currentTTY->buffer;
	buffer->first = buffer->first->next;
	buffer->last = buffer->last->next;

	bigBzero((uint16_t *)&(buffer->last->buffer[0]), MAX_COLUMNS);
	putTtyBuffer();
}

void updatePositionY(_tty *tty)
{
	if (tty->bufferSize >= MAX_ROWS)
		scroll();
	else
	{
		tty->bufferSize++;
		tty->posY++;
		tty->buffer->last = tty->buffer->last->next;
	}
}

void updatePositionX(_tty *tty)
{
	tty->posX++;
	if (tty->posX >= (MAX_COLUMNS - 1))
	{
		tty->posX = 0;
		updatePositionY(tty);
	}
}

uint8_t putChar(char c)
{
	_tty *tty;
	uint8_t ret;

	tty = g_terminal.currentTTY;

	tty->buffer->last->buffer[tty->posX].character = c;
	tty->buffer->last->buffer[tty->posX].color = 0x07;

	if (c == '\n')
	{
		// to do: call prompt ??
		if (tty->posX)
		{
			tty->posX = 0;
			updatePositionY(tty);
		}
		return (1);
	}

	ret = putCharPos(c, tty->posX, tty->posY);

	if (!ret)
		return (0);

	updatePositionX(tty);

	// putTtyBuffer();

	// putCharPos('+', tty->posX, tty->posY);
	setCursor(tty->posX, tty->posY);
	return (1);
}

void setCursor(uint8_t x, uint8_t y)
{
	uint32_t pos = y * MAX_COLUMNS + x;

	// pos = 1;
	portByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
	portByteOut(VGA_DATA_REGISTER, (unsigned char)(pos >> 8));

	portByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
	portByteOut(VGA_DATA_REGISTER, (unsigned char)(pos & 0xff));
}

