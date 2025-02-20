#include "terminal/terminal.h"
#include "terminal/vga.h"
#include "terminal/tty.h"
#include "klibc/converts.h"
#include "klibc/strings.h"
#include "images/RGB.h"

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
	else {
		for (uint8_t i = 0; i < 16; i++){
			if (ansiNbr == g_ansi[i]) {
				g_currentTextColor = i;
				break;
			}
			else if (ansiNbr == (g_ansi[i] + 10)) {
				g_currentBackGroundColor = i;
				break;
			}
		}
	}
}

uint8_t isColor(char c){
	// to do: 100 max for now can realloc later
	static char color[100];
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
			do {
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

static void swap(_vgaCell *a, _vgaCell *b){
	_vgaCell c;

	c = *a;
	*a = *b;
	*b = c;
}


void slideBufferRight( void ){
	_node		*ptr;
	size_t		x, y;
	_vgaCell	*str;
	_vgaCell	c;

	y = CURRENT_TTY->cursorY;
	x = CURRENT_TTY->cursorX + 1;
	ptr = CURRENT_TTY->buffer->current;
	c = ((_vgaCell *)ptr->ptr)[CURRENT_TTY->cursorX];
	while (y++ <= CURRENT_TTY->posY){
		str = ptr->ptr;
		for (; (x < MAX_COLUMNS) && c.character; x++)
			swap(&c, &(str[x]));
		str[x].character = 0;
		x = 0;
		ptr = ptr->next;
	}
	putPartOfBuffer(CURRENT_TTY->cursorX + 1);
}

void slideBufferLeft( void ){
	_node		*ptr;
	size_t		x, y;
	_vgaCell	*str;

	y = CURRENT_TTY->cursorY;
	x = CURRENT_TTY->cursorX;
	ptr = CURRENT_TTY->buffer->current;
	while (y++ <= CURRENT_TTY->posY){
		str = ptr->ptr;
		for (; (x < (MAX_COLUMNS - 1)) && str[x + 1].character; x++)
			str[x] = str[x + 1];
		str[x].character = 0;
		ptr = ptr->next;
		if (x == (MAX_COLUMNS - 1))
			str[x] = ((_vgaCell *)(ptr->ptr))[0];
		x = 0;
	}

	putPartOfBuffer(CURRENT_TTY->cursorX);
	if (CURRENT_TTY->mode->putPixel)
		drawFilledRectangle((_positionPair){CURRENT_TTY->posX * FONT_WIDTH, CURRENT_TTY->posY * FONT_HEIGHT}, FONT_WIDTH, FONT_HEIGHT, DEFAULT_BACKGROUND_COLOR);
	else
		putCharPos(' ', CURRENT_TTY->posX, CURRENT_TTY->posY);
}

uint8_t putChar(char c){
	_vgaCell	*buffer;

	if (isColor(c))
		return (0);

	updateCursorData(0);
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
			buffer = CURRENT_TTY->buffer->current->ptr;
			if (CURSOR_AT_THE_END){
				putCharPos(' ', CURRENT_TTY->cursorX, CURRENT_TTY->cursorY);
				buffer[CURRENT_TTY->cursorX].character = 0;
			}
			else
				slideBufferLeft();
			setCursor();
			return (1);
		default:
			break;
	}

	if (!CURSOR_AT_THE_END)
		slideBufferRight();
	
	if (!putCharPos(c, CURRENT_TTY->cursorX, CURRENT_TTY->cursorY))
		return (0);

	buffer = CURRENT_TTY->buffer->current->ptr;

	buffer[CURRENT_TTY->cursorX].character = c;
	buffer[CURRENT_TTY->cursorX].color = g_currentTextColor;
	buffer[CURRENT_TTY->cursorX].color |= g_currentBackGroundColor << 4;

	incrementPositionX();
	incrementCursorX();

	setCursor();

	return (1);
}