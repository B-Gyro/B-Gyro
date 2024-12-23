#include "terminal/terminal.h"
#include "terminal/vga.h"
#include "terminal/tty.h"
#include "klibc/converts.h"
#include "klibc/print.h"
#include "klibc/strings.h"

uint8_t isColor(char c);

uint8_t putStrPos(char *str, uint32_t x, uint32_t y){
	uint32_t i = 0;

	while (str[i]){
		if (putCharPos(str[i], x, y))
			x++;
		if (x >= MAX_COLUMNS)
			x = 0, y++;
		i++;
	}
	return (1);
}

uint8_t putCharPos(char c, uint32_t x, uint32_t y){
	_vgaCell cell;

	if (x >= MAX_COLUMNS || y > MAX_ROWS)
		return (0);

	// to do : only test for now
	if (isColor(c))
		return (0);

	cell.character = c;

	cell.color = g_currentTextColor;
	cell.color |= g_currentBackGroundColor << 4;

	putCellOnVga(cell, x, y);
	return (1);
}

void setVgaColor(uint8_t ansiNbr){
	if (!ansiNbr){
		g_currentTextColor = DEFAULT_TEXT_COLOR;
		g_currentBackGroundColor = DEFAULT_BACKGROUND_COLOR;
	}
	else if (ansiNbr == 39)
		g_currentTextColor = DEFAULT_TEXT_COLOR;
	else if (ansiNbr == 49)
		g_currentBackGroundColor = DEFAULT_BACKGROUND_COLOR;
	else{
		for (uint8_t i = 0; i < 16; i++){
			if (ansiNbr == g_ansi[i])
				g_currentTextColor = i;
			else if (ansiNbr == (g_ansi[i] + 10))
				g_currentBackGroundColor = i;
		}
	}
}

uint8_t isColor(char c){
	// to do: 20 max for now can realloc later
	static char color[20];
	static size_t i = 1;

	if (color[0]){
		if ((i > 19) ||
			((i == 1) && (c != '[')) ||
			((i > 1) && !(isDigit(c) || c == ';' || c == 'm'))){

			i = 1;
			color[0] = 0;
			return (1);
		}

		color[i] = c;
		if (color[i] == 'm'){
			i = 2;
			do
			{
				setVgaColor(uatoi(color, &i));
			} while (color[i++] != 'm');

			i = 1;
			color[0] = 0;
			return (1);
		}
		i++;
		return (1);
	}

	if (c == '\033'){
		color[0] = c;
		return (1);
	}

	return (0);
}

void swap(_vgaCell *a, _vgaCell *b){
	_vgaCell c;

	c = *a;
	*a = *b;
	*b = c;
}

void slideBufferRight( void ){
	_node		*ptr;
	size_t		x, y, i;
	_vgaCell	*str;
	_vgaCell	c;

	y = CURRENT_TTY->cursorY;
	x = CURRENT_TTY->cursorX + 1;
	ptr = CURRENT_TTY->buffer->current;
	c = ((_vgaCell *)ptr->ptr)[CURRENT_TTY->cursorX];
	while (y++ <= CURRENT_TTY->posY){
		str = ptr->ptr;
		for (i = x; (i < MAX_COLUMNS) && c.character; i++)
			swap(&c, &(str[i]));
		x = 0;
		ptr = ptr->next;
	}
}

void slideBufferLeft( void ){
	_node		*ptr;
	size_t		x, y, i;
	_vgaCell	*str;

	y = CURRENT_TTY->cursorY;
	x = CURRENT_TTY->cursorX;
	ptr = CURRENT_TTY->buffer->current;
	while (y++ <= CURRENT_TTY->posY){
		str = ptr->ptr;
		for (i = x; i < (MAX_COLUMNS - 1); i++)
			str[i] = str[i + 1];
		ptr = ptr->next;
		if (i == (MAX_COLUMNS - 1))
			str[i] = ((_vgaCell *)(ptr->ptr))[0];
		x = 0;
	}
}

uint8_t putChar(char c){
	_vgaCell	*buffer;

	if (isColor(c))
		return (0);

	buffer = CURRENT_TTY->buffer->current->ptr;
	switch (c){
		case '\n':
			if (CURRENT_TTY->posX){
				incrementPositionY();
				incrementCursorY();
			}
			// to do: remove this line later
			CURRENT_TTY->posX = 0;
			CURRENT_TTY->cursorX = 0;
			setCursor();
			return (1);
		case '\r':
			CURRENT_TTY->cursorX = 0;
			setCursor();
			return (1);
		case '\t':
			for (uint8_t i = 0; i < TAB_SIZE; i++)
				putChar(' ');
			return (1);
		case '\b':
			decrementPositionX();
			decrementCursorX();
			if (CURSOR_AT_THE_END) {
				putCharPos(' ', CURRENT_TTY->cursorX, CURRENT_TTY->cursorY);
				buffer[CURRENT_TTY->cursorX].character = 0;
				setCursor();
			}
			else {
				slideBufferLeft();
				putTtyBuffer();
			}
			return (1);
		default:
			break;
	}

	if (!CURSOR_AT_THE_END){
		slideBufferRight();
	}
	else if (!putCharPos(c, CURRENT_TTY->cursorX, CURRENT_TTY->cursorY))
		return (0);


	buffer[CURRENT_TTY->cursorX].character = c;
	buffer[CURRENT_TTY->cursorX].color = g_currentTextColor;
	buffer[CURRENT_TTY->cursorX].color |= g_currentBackGroundColor << 4;

	incrementPositionX();
	incrementCursorX();

	if (!CURSOR_AT_THE_END) {
		putTtyBuffer();
	}
	else
		setCursor();

	return (1);
}