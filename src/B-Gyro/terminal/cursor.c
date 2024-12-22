#include "terminal/terminal.h"
#include "terminal/vga.h"
#include "terminal/tty.h"
#include "klibc/memory.h"

void	decrementCursorY(_tty *tty){
	if (!tty->cursorY)
		return;
	tty->cursorY--;
	tty->buffer->current = tty->buffer->current->previous;
}

void	incrementCursorY(_tty *tty){
	if ((tty->cursorY > (tty->posY + 1)) ||
		(tty->cursorY >= (MAX_ROWS - 1)))
		return;
	tty->cursorY++;
	tty->buffer->current = tty->buffer->current->next;
}

void	decrementCursorX(_tty *tty){
	if (!tty->cursorX){
		if (!tty->cursorY)
			return;

		tty->cursorX = MAX_COLUMNS - 1;
		decrementCursorY(tty);
	}
	else
		tty->cursorX--;
}

void	incrementCursorX(_tty *tty){
	tty->cursorX++;
	if (tty->cursorX >= MAX_COLUMNS){
		tty->cursorX = 0;
		incrementCursorY(tty);
	}
}

void	setCursor(void){
	uint32_t y = g_terminal.currentTTY->cursorY;
	uint32_t x = g_terminal.currentTTY->cursorX;
	uint32_t pos = y * MAX_COLUMNS + x;

	if (!((_vgaCell *)VIDEO_ADDRESS)[pos].character)
		putCharPos(' ', x, y);
	portByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
	portByteOut(VGA_DATA_REGISTER, (unsigned char)(pos >> 8));

	portByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
	portByteOut(VGA_DATA_REGISTER, (unsigned char)(pos & 0xff));
}

void	moveCursorRight(_tty *tty){
	if (tty->keyboardBuffer.index == tty->keyboardBuffer.size)
		return;
	incrementCursorX(tty);
	tty->keyboardBuffer.index++;
	setCursor();
}

void	moveCursorLeft(_tty *tty){
	if (!tty->keyboardBuffer.index)
		return;
	decrementCursorX(tty);
	tty->keyboardBuffer.index--;
	setCursor();
}

void	resetCursor(_tty *tty){
	tty->cursorX = tty->posX;
	tty->cursorY = tty->posY;
}
