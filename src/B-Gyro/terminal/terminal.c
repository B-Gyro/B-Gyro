#include "terminal/terminal.h"
#include "terminal/vga.h"
#include "terminal/tty.h"
#include "klibc/memory.h"
#include "klibc/strings.h"
#include "images/image.h"

void initTerminal(){


	CURRENT_TTY = g_terminal.ttys;

	// -_-
	for (uint8_t i = 0; i < MAX_TTYS; i++)
	{
		g_terminal.ttys[i].buffer = g_buffers + i;
		g_terminal.ttys[i].history = g_histories + i;
	}
	initTTY(0);
	initUsers();
	SERIAL_SUCC("Terminal Initialized");
}

static void	scroll(void){
	_list *buffer;

	buffer = CURRENT_TTY->buffer;
	buffer->first = buffer->first->next;
	buffer->last = buffer->last->next;
	buffer->current = buffer->current->next;

	bigBzero(buffer->last->ptr, _MAX_COLUMNS);
	putTtyBuffer();
}

void decrementPositionY( void )
{
	if (!CURRENT_TTY->posY)
		return;
	else{
		CURRENT_TTY->posY--;
		CURRENT_TTY->buffer->size--;
		CURRENT_TTY->buffer->last = CURRENT_TTY->buffer->last->previous;
		CURRENT_TTY->posX = MAX_COLUMNS - 1;
	}
}

void incrementPositionY( void ){
	if (CURRENT_TTY->buffer->size >= MAX_ROWS){
		if (!CURSOR_AT_THE_END)
			decrementCursorY();
		scroll();
	}
	else{
		CURRENT_TTY->buffer->size++;
		CURRENT_TTY->posY++;
		bigBzero(CURRENT_TTY->buffer->last->next->ptr, _MAX_COLUMNS);
		CURRENT_TTY->buffer->last = CURRENT_TTY->buffer->last->next;
	}
}

void decrementPositionX( void ){
	if (!CURRENT_TTY->posX){
		if (!CURRENT_TTY->posY)
			return;
		CURRENT_TTY->posX = MAX_COLUMNS - 1;
		decrementPositionY();
	}
	else
		CURRENT_TTY->posX--;
}

void incrementPositionX( void ){
	CURRENT_TTY->posX++;
	if (CURRENT_TTY->posX >= MAX_COLUMNS){
		CURRENT_TTY->posX = 0;
		incrementPositionY();
	}
}

void putCellOnVga(_vgaCell cell, uint8_t x, uint8_t y){
	if (!CURRENT_TTY->mode || !(CURRENT_TTY->mode->putPixel)) {
		uint32_t pos;
		_vgaCell *adress = (_vgaCell *)VIDEO_ADDRESS;

		pos = y * MAX_COLUMNS + x;
		adress[pos] = cell;
	}
	else
		drawCharacter(cell, x * FONT_WIDTH, y * FONT_HEIGHT);
}
