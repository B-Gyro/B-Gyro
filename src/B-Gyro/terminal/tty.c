#include "terminal/_data.h"
#include "terminal/vga.h"
#include "terminal/tty.h"
#include "terminal/terminal.h"
#include "klibc/memory.h"
#include "klibc/strings.h"
#include "sshell/sshell.h"
#include "bGyro.h"
#include "arch/i386/pit.h"
#include "images/image.h"

extern _vgaMode g_T80x25;

void initTTY(uint8_t index){
	_tty *tty = CURRENT_TTY;
	_node *ptr;

	tty->posX = 0;
	tty->posY = 0;
	tty->cursorX = 0;
	tty->cursorY = 0;

	tty->mode = &g_T80x25;
	tty->font = &g_fontText;

	tty->index = index;

	tty->textColor = DEFAULT_TEXT_COLOR;
	tty->backgroundColor = DEFAULT_BACKGROUND_COLOR;

	tty->buffer->first = &g_rows[index][0];
	ptr = tty->buffer->first;
	for (uint8_t i = 0; i < MAX_ROWS; i++){
		ptr->ptr = &g_ttyBuffers[index][i];
		ptr->next = &g_rows[index][(i + 1) % MAX_ROWS];
		if (i)
			ptr->previous = &g_rows[index][i - 1];
		ptr = ptr->next;
	}
	tty->buffer->first->previous = &g_rows[index][MAX_ROWS - 1];
	tty->buffer->last = tty->buffer->first;
	tty->buffer->current = tty->buffer->first;

	CURRENT_TTY->buffer->size = 1;
	initHistory();
	updateStatusBar();
	SERIAL_SUCC("TTY %d Initialized", index);
}

void clearTTY(uint32_t size){
	clearVGA(size);

	CURRENT_TTY->cursorX = 0;
	CURRENT_TTY->cursorY = 0;

	CURRENT_TTY->buffer->size = 1;
	bigBzero(CURRENT_TTY->buffer->first->ptr, MAX_COLUMNS);
	CURRENT_TTY->buffer->last = CURRENT_TTY->buffer->first;
	CURRENT_TTY->buffer->current = CURRENT_TTY->buffer->first;

	if (size == FULL_SCREEN_SIZE)
		bigBzero(CURRENT_TTY->status, MAX_COLUMNS);
}

void putTtyBuffer(void){
	_node *line;

	line = CURRENT_TTY->buffer->first;

	clearVGA(0);

	for (CURRENT_TTY->posY = 0; CURRENT_TTY->posY < CURRENT_TTY->buffer->size; CURRENT_TTY->posY++){
		for (CURRENT_TTY->posX = 0; CURRENT_TTY->posX < MAX_COLUMNS; CURRENT_TTY->posX++){
			if (((_vgaCell *)line->ptr)[CURRENT_TTY->posX].character == '\0' ||
				((_vgaCell *)line->ptr)[CURRENT_TTY->posX].character == '\n')
				break;
			putCellOnVga(((_vgaCell *)line->ptr)[CURRENT_TTY->posX], CURRENT_TTY->posX, CURRENT_TTY->posY);
		}
		line = line->next;
	}
	CURRENT_TTY->posY--;
	if (CURRENT_TTY->posX >= MAX_COLUMNS){
		incrementPositionX();
		putTtyBuffer();
	}
	setCursor();
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
	
	if (!(((char *)CURRENT_TTY->buffer->first->ptr)[0]))
		interruptPrompting();

	updateStatusBar();

	g_currentTextColor = tty->textColor;
	g_currentBackGroundColor = tty->backgroundColor;
	putTtyBuffer();
}


/*------------------------------ STATUS BAR ------------------------------*/

void clearStatusBar(void){
	if (CURRENT_TTY->mode->putPixel){
		for (size_t i = 0; i < CURRENT_TTY->mode->screenWidth; i++)
				CURRENT_TTY->mode->putPixel((_positionPair){i, MAX_ROWS}, g_currentBackGroundColor);
	}
	else
		bigBzero((uint16_t *)CURRENT_TTY->mode->VMStart + MAX_ROWS * MAX_COLUMNS, MAX_COLUMNS);
	bigBzero(CURRENT_TTY->status, MAX_COLUMNS);
}

void updateStatusBar(void){
	char content[80];

	clearStatusBar();
	SPRINTF(content, "TTY: %d | OSVersion: " COLOR_LIGHT_CYAN "%s" COLOR_DEFAULT " | STATE: %7s | SERIAL: %8s | ",
			CURRENT_TTY->index + 1,
			g_bGyroStats.OSVersion,
			bGyroStatusToString(g_bGyroStats.status),
			g_bGyroStats.hasSerialWorking ? "ENABLED" : "DISABLED");
	putStrPos(content, 0, MAX_ROWS);
	printTimer();
}

/*------------------------------------------------------------------------*/
