#include "terminal/terminal.h"
#include "terminal/vga.h"
#include "terminal/tty.h"
#include "klibc/memory.h"

void	decrementCursorY( void ){
	if (!CURRENT_TTY->cursorY)
		return;
	CURRENT_TTY->cursorY--;
	CURRENT_TTY->buffer->current = CURRENT_TTY->buffer->current->previous;
}

void	incrementCursorY( void ){
	if ((CURRENT_TTY->cursorY > (CURRENT_TTY->posY + 1)) ||
		(CURRENT_TTY->cursorY >= (MAX_ROWS - 1)))
		return;
	CURRENT_TTY->cursorY++;
	CURRENT_TTY->buffer->current = CURRENT_TTY->buffer->current->next;
}

void	decrementCursorX( void ){
	if (!CURRENT_TTY->cursorX){
		if (!CURRENT_TTY->cursorY)
			return;

		CURRENT_TTY->cursorX = MAX_COLUMNS - 1;
		decrementCursorY();
	}
	else
		CURRENT_TTY->cursorX--;
}

void	incrementCursorX( void ){
	CURRENT_TTY->cursorX++;
	if (CURRENT_TTY->cursorX >= MAX_COLUMNS){
		CURRENT_TTY->cursorX = 0;
		incrementCursorY();
	}
}

void	updateCursorData(bool draw){
	static char	hovered = 0;
	uint32_t y, x;

	if (!CURRENT_TTY->mode->putPixel)
		return ;
	
	y = CURRENT_TTY->cursorY * FONT_HEIGHT;
	x = CURRENT_TTY->cursorX * FONT_WIDTH;
	if (!draw) {
		if (hovered)
			putCharPos(hovered, CURRENT_TTY->cursorX, CURRENT_TTY->cursorY);
		else
			drawFilledRectangle(x, y, FONT_WIDTH, FONT_HEIGHT, g_currentBackGroundColor);
		hovered = 0;
		return ;
	}

	hovered = ((_vgaCell *)CURRENT_TTY->buffer->current->ptr)[CURRENT_TTY->cursorX].character;
	drawFilledRectangle(x, y, FONT_WIDTH, FONT_HEIGHT, CURSOR_COLOR);
}

void setCursor(void){
	uint32_t	pos;

	if (CURRENT_TTY->mode->putPixel)
		updateCursorData(1);
	else {
		pos = CURRENT_TTY->cursorY * MAX_COLUMNS + CURRENT_TTY->cursorX;
		if (!((_vgaCell *)VIDEO_ADDRESS)[pos].character)
			putCharPos(' ', CURRENT_TTY->cursorX, CURRENT_TTY->cursorY);
		portByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
		portByteOut(VGA_DATA_REGISTER, (unsigned char)(pos >> 8));

		portByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
		portByteOut(VGA_DATA_REGISTER, (unsigned char)(pos & 0xff));
	}
}

void	cursor(bool b);

void moveCursorRight( void ){
	if (CURRENT_TTY->keyboardBuffer.index == CURRENT_TTY->keyboardBuffer.size)
		return;
	updateCursorData(0);
	incrementCursorX();
	CURRENT_TTY->keyboardBuffer.index++;
	setCursor();
}

void moveCursorLeft( void ){
	if (!CURRENT_TTY->keyboardBuffer.index)
		return;
	updateCursorData(0);
	decrementCursorX();
	CURRENT_TTY->keyboardBuffer.index--;
	setCursor();
}

void resetCursor( void ){
	CURRENT_TTY->buffer->current = CURRENT_TTY->buffer->last;
	CURRENT_TTY->cursorX = CURRENT_TTY->posX;
	CURRENT_TTY->cursorY = CURRENT_TTY->posY;
}

void	updateCursorLoc(size_t x, size_t y){
	CURRENT_TTY->cursorX = x;
	CURRENT_TTY->posX = x;
	CURRENT_TTY->cursorY = y;
	CURRENT_TTY->posY = y;
	setCursor();
}