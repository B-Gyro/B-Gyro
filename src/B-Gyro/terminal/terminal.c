#include "terminal/terminal.h"
#include "terminal/vga.h"
#include "klibc/memory.h"

void putCellOnVga(_vgaCell cell, uint8_t x, uint8_t y) {
	uint32_t pos;
	_vgaCell *adress = (_vgaCell *)VIDEO_ADDRESS;

	pos = y * MAX_COLUMNS + x;
	adress[pos] = cell;
}


void scroll(void) {
	_list *buffer;

	buffer = g_terminal.currentTTY->buffer;
	buffer->first = buffer->first->next;
	buffer->last = buffer->last->next;

	bigBzero((uint16_t *)&(buffer->last->buffer[0]), MAX_COLUMNS);
	putTtyBuffer();
}

void updatePositionY(_tty *tty) {
	if (tty->bufferSize >= MAX_ROWS)
		scroll();
	else
	{
		tty->bufferSize++;
		tty->posY++;
		tty->buffer->last = tty->buffer->last->next;
	}
}

void updatePositionX(_tty *tty) {
	tty->posX++;
	if (tty->posX >= MAX_COLUMNS) {
		tty->posX = 0;
		updatePositionY(tty);
	}
}


void setCursor(uint8_t x, uint8_t y) {
	uint32_t pos = y * MAX_COLUMNS + x;

	portByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
	portByteOut(VGA_DATA_REGISTER, (unsigned char)(pos >> 8));

	portByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
	portByteOut(VGA_DATA_REGISTER, (unsigned char)(pos & 0xff));
}

