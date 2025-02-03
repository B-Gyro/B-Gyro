#include "bGyro.h"
#include "terminal/vga.h"
#include "terminal/tty.h"
#include "terminal/terminal.h"
#include "terminal/_data.h"
#include "klibc/memory.h"
#include "klibc/strings.h"
#include "sshell/sshell.h"
#include "time/pit.h"
#include "images/image.h"

extern _vgaMode g_T80x25;
bool	g_shellMode = 1;

void initTTY(uint8_t index){
	_tty *tty = CURRENT_TTY;
	_node *ptr;

	tty->posX = 0;
	tty->posY = 0;
	tty->cursorX = 0;
	tty->cursorY = 0;

	// default mode
	changeVGAModeT80x25();

	tty->index = index;

	tty->textColor = DEFAULT_TEXT_COLOR;
	tty->backgroundColor = DEFAULT_BACKGROUND_COLOR;

	tty->buffer->first = &g_rows[index][0];
	ptr = tty->buffer->first;
	for (uint8_t i = 0; i < _MAX_ROWS; i++){
		ptr->ptr = &g_ttyBuffers[index][i];
		ptr->next = &g_rows[index][(i + 1) % _MAX_ROWS];
		if (i)
			ptr->previous = &g_rows[index][i - 1];
		ptr = ptr->next;
	}
	tty->buffer->first->previous = &g_rows[index][_MAX_ROWS - 1];
	tty->buffer->last = tty->buffer->first;
	tty->buffer->current = tty->buffer->first;

	CURRENT_TTY->buffer->size = 1;
	initHistory();
	updateStatusBar();
	SERIAL_SUCC("TTY %d Initialized", index);
}

void clearTTY(bool fullScreen){
	CURRENT_TTY->mode->clearScreen(fullScreen);

	CURRENT_TTY->mode->clearScreen(0);

	CURRENT_TTY->cursorX = CURRENT_TTY->cursorY = 0;
	CURRENT_TTY->posX = CURRENT_TTY->posY = 0;

	CURRENT_TTY->buffer->size = 1;
	bigBzero(CURRENT_TTY->buffer->first->ptr, _MAX_COLUMNS);
	CURRENT_TTY->buffer->last = CURRENT_TTY->buffer->first;
	CURRENT_TTY->buffer->current = CURRENT_TTY->buffer->first;

	if (fullScreen)
		bigBzero(CURRENT_TTY->status, _MAX_COLUMNS);
}

void putPartOfBuffer(uint32_t cursorX){
	_node		*line;
	size_t		x, y;

	x = cursorX;
	line = CURRENT_TTY->buffer->current;

	for (y = CURRENT_TTY->cursorY; y <= CURRENT_TTY->posY; y++){
		for (; x < MAX_COLUMNS; x++){
			if (((_vgaCell *)line->ptr)[x].character == '\0' ||
				((_vgaCell *)line->ptr)[x].character == '\n')
				break;
			putCellOnVga(((_vgaCell *)line->ptr)[x], x, y);
		}
		x = 0;
		line = line->next;
	}
}

void putTtyBuffer(void){
	_node *line;
	size_t	size;
	line = CURRENT_TTY->buffer->first;

	CURRENT_TTY->mode->clearScreen(0);

	size = CURRENT_TTY->buffer->size;
	if (size > MAX_ROWS) // size may be bigger if we switched from bigger vga mode
		size = MAX_ROWS;
	for (CURRENT_TTY->posY = 0; CURRENT_TTY->posY < size; CURRENT_TTY->posY++){
		for (CURRENT_TTY->posX = 0; CURRENT_TTY->posX < MAX_COLUMNS; CURRENT_TTY->posX++){
			if (((_vgaCell *)line->ptr)[CURRENT_TTY->posX].character == '\0' ||
				((_vgaCell *)line->ptr)[CURRENT_TTY->posX].character == '\n')
				break;
			putCellOnVga(((_vgaCell *)line->ptr)[CURRENT_TTY->posX], CURRENT_TTY->posX, CURRENT_TTY->posY);
		}
		line = line->next;
	}
	CURRENT_TTY->posY--;
	// if (CURRENT_TTY->posX >= MAX_COLUMNS){
	// 	incrementPositionX();
	// 	putTtyBuffer();
	// }
	// setCursor();
}

void	switchTTY(uint8_t index){
	_tty *tty;

	if ((index >= MAX_TTYS) || \
		(index == CURRENT_TTY->index) || \
		!(g_users.current))
		return;

	CURRENT_TTY->textColor = g_currentTextColor;
	CURRENT_TTY->backgroundColor = g_currentBackGroundColor;

	tty = &(g_terminal.ttys[index]);
	CURRENT_TTY = &(g_terminal.ttys[index]);

	keyboardSetBuffer(&(g_terminal.ttys[index].keyboardBuffer), 0);

	if (tty->index != index)
		initTTY(index);
	else
		CURRENT_TTY->mode->func();
	
	if (!(((char *)CURRENT_TTY->buffer->first->ptr)[0]))
		interruptPrompting();

	updateStatusBar();

	g_currentTextColor = tty->textColor;
	g_currentBackGroundColor = tty->backgroundColor;
	putTtyBuffer();
	setCursor();
}


/*------------------------------ STATUS BAR ------------------------------*/

void clearStatusBar(void){
	_vgaCell	cell;
	uint32_t	size;

	if (CURRENT_TTY->mode->putPixel){
		for (size_t j = 0; j <= FONT_HEIGHT; j++)
			for (size_t i = 0; i < CURRENT_TTY->mode->screenWidth; i++)
					CURRENT_TTY->mode->putPixel((_positionPair){i, (MAX_ROWS * FONT_HEIGHT) + j}, DEFAULT_BACKGROUND_COLOR);
	}
	else {
		cell.character = ' ';
		cell.color = DEFAULT_BACKGROUND_COLOR << 4;
		size = MAX_ROWS * MAX_COLUMNS;
		for (size_t j = 0; j < CURRENT_TTY->mode->screenWidth; j++)
			((_vgaCell *)(VIDEO_ADDRESS))[size + j] = cell;
	}
	bigBzero(CURRENT_TTY->status, _MAX_COLUMNS);
	updateTime(1);
}

void updateStatusBar(void){
	char content[80];

	clearStatusBar();

	SPRINTF(content, "TTY: %d | OSVersion: " COLOR_LIGHT_CYAN "%s " COLOR_RESET " | STATE: %7s | SERIAL: %8s | ",
			CURRENT_TTY->index + 1,
			g_bGyroStats.OSVersion,
			bGyroStatusToString(g_bGyroStats.status),
			g_bGyroStats.hasSerialWorking ? "ENABLED" : "DISABLED");
	putStrPos(content, 0, MAX_ROWS);
	// updateTime(1);
}

/*------------------------------------------------------------------------*/
