#include "terminal/terminal.h"
#include "terminal/vga.h"
#include "klibc/converts.h"
#include "klibc/print.h"
#include "klibc/strings.h"

uint8_t isColor(char c);

uint8_t putStrPos(char *str, uint32_t x, uint32_t y)
{
	uint32_t i = 0;

	while (str[i])
	{
		if (putCharPos(str[i], x, y))
			x++;
		if (x >= MAX_COLUMNS)
			x = 0, y++;
		i++;
	}
	return (1);
}

uint8_t putCharPos(char c, uint32_t x, uint32_t y)
{
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

void setVgaColor(uint8_t ansiNbr)
{
	if (!ansiNbr)
	{
		g_currentTextColor = DEFAULT_TEXT_COLOR;
		g_currentBackGroundColor = DEFAULT_BACKGROUND_COLOR;
	}
	else if (ansiNbr == 39)
		g_currentTextColor = DEFAULT_TEXT_COLOR;
	else if (ansiNbr == 49)
		g_currentBackGroundColor = DEFAULT_BACKGROUND_COLOR;
	else
	{
		for (uint8_t i = 0; i < 16; i++)
		{
			if (ansiNbr == g_ansi[i])
				g_currentTextColor = i;
			else if (ansiNbr == (g_ansi[i] + 10))
				g_currentBackGroundColor = i;
		}
	}
}

uint8_t isColor(char c)
{
	// to do: 20 max for now can realloc later
	static char color[20];
	static size_t i = 1;

	if (color[0])
	{
		if ((i > 19) ||
			((i == 1) && (c != '[')) ||
			((i > 1) && !(isDigit(c) || c == ';' || c == 'm')))
		{

			i = 1;
			color[0] = 0;
			return (1);
		}

		color[i] = c;
		if (color[i] == 'm')
		{
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

	if (c == '\033')
	{
		color[0] = c;
		return (1);
	}

	return (0);
}
void putTtyBuffer(void);

void	swap(_vgaCell *a, _vgaCell *b) {
	_vgaCell	c;

	c = *a;
	*a = *b;
	*b = c;
}

void	slideBuffer(_tty *tty){
	_node	*ptr = tty->buffer->current;
	_vgaCell	c;
	_vgaCell	*str;
	uint32_t	y = tty->cursorY, x = tty->cursorX;

	str = ptr->ptr;
	c = str[x];
	x++;
	while (y++ <= tty->posY)
	{
		str = ptr->ptr;
		for (size_t i = x; (i < MAX_COLUMNS) && c.character; i++) {
			// SERIAL_PRINT("- %c, %c\n", str[i], c);
			swap(&c, &(str[i]));
			// SERIAL_PRINT("- %c, %c\n", str[i], c);
		}
		x = 0;
		ptr = ptr->next;
	}
}

uint8_t putChar(char c)
{
	_tty *tty;
	_node *last;
	uint8_t ret;
	// _vgaCell *buffer = last->ptr;

	if (isColor(c))
		return (0);

	tty = g_terminal.currentTTY;
	last = tty->buffer->current;

	switch (c)
	{
	case '\n':
		if (tty->posX)
		{
			incrementPositionY(tty);
			incrementCursorY(tty);
		}
		// to do: remove this line later
		tty->posX = 0;
		tty->cursorX = 0;
		setCursor();
		return (1);
	case '\r':
		tty->cursorX = 0;
		setCursor();
		return (1);
	case '\t':
		for (uint8_t i = 0; i < TAB_SIZE; i++)
			putChar(' ');
		return (1);
	case '\b':
		decrementPositionX(tty);
		decrementCursorX(tty);
		putCharPos(' ', tty->cursorX, tty->cursorY);
		((_vgaCell *)last->ptr)[tty->cursorX].character = 0;
		setCursor();
		return (1);
	default:
		break;
	}

	if (tty->cursorX != tty->posX) {
		// if (tty->posX == (MAX_COLUMNS - 1) && 
		// tty->posY == (MAX_ROWS - 1)){
		// 	scroll();}
		slideBuffer(tty);
	}

	((_vgaCell *)last->ptr)[tty->cursorX].character = c;
	((_vgaCell *)last->ptr)[tty->cursorX].color = g_currentTextColor;
	((_vgaCell *)last->ptr)[tty->cursorX].color |= g_currentBackGroundColor << 4;

	ret = putCharPos(c, tty->cursorX, tty->cursorY);

	if (!ret)
		return (0);

	incrementPositionX(tty);
	incrementCursorX(tty);
	setCursor();

	putTtyBuffer();
	return (1);
}