#include "terminal/terminal.h"
#include "terminal/vga.h"
#include "klibc/converts.h"
#include "klibc/print.h"
// #include "klibc/memory.h"

uint8_t putCharPos(char c, uint32_t x, uint32_t y)
{
	_vgaCell cell;

	if (x >= MAX_COLUMNS || y > MAX_ROWS)
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

uint8_t putChar(char c)
{
	_tty *tty;
	uint8_t ret;

	if (isColor(c))
		return (0);

	tty = g_terminal.currentTTY;

	tty->buffer->last->buffer[tty->posX].character = c;
	tty->buffer->last->buffer[tty->posX].color = g_currentTextColor;
	tty->buffer->last->buffer[tty->posX].color |= g_currentBackGroundColor << 4;

	switch (c)
	{
	case '\n':
		if (tty->posX)
			incrementPositionY(tty);
		// to do: remove this line later
		tty->posX = 0;
		setCursor(tty->posX, tty->posY);
		return (1);
	case '\r':
		tty->posX = 0;
		setCursor(tty->posX, tty->posY);
		return (1);
	case '\t':
		for (uint8_t i = 0; i < TAB_SIZE; i++)
			putChar(' ');
		return (1);
	case '\b':
		if (!tty->posX)
			decrementPositionY(tty);
		else
			tty->posX--;
		putCharPos(' ', tty->posX, tty->posY);
		tty->buffer->last->buffer[tty->posX].character = ' ';
		setCursor(tty->posX, tty->posY);
		return (1);
	default:
		break;
	}

	ret = putCharPos(c, tty->posX, tty->posY);

	if (!ret)
		return (0);

	incrementPositionX(tty);

	putCharPos(' ', tty->posX, tty->posY);
	setCursor(tty->posX, tty->posY);
	return (1);
}