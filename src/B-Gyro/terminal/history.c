#include "terminal/_data.h"
#include "terminal/tty.h"
#include "klibc/memory.h"
# include "drivers/keyboard.h"
#include "klibc/strings.h"

void	getHistory(uint8_t cursor) {
	_list	*history = g_terminal.currentTTY->history;
	_tty	*tty = g_terminal.currentTTY;
	
	if (cursor == CURSOR_UP) {
		if (history->current == history->first)
			return ;
		if (history->current == history->last) 
			safeStrcpy(history->last->ptr, tty->keyboardBuffer.buffer, MAX_KEYBOARD_BUFFER);
		history->current = history->current->previous;
	}
	else if (cursor == CURSOR_DOWN) {
		if (history->current == history->last)
			return ;
		history->current = history->current->next;
	}

	// to do: do it better way !!
	for (uint32_t i = 0; i < tty->keyboardBuffer.size; i++)
		putChar('\b');

	tty->keyboardBuffer.size = safeStrcpy(tty->keyboardBuffer.buffer, history->current->ptr, MAX_KEYBOARD_BUFFER);
	tty->keyboardBuffer.index = tty->keyboardBuffer.size - 1;
	VGA_PRINT("%s", tty->keyboardBuffer.buffer);
}

void	printHistory( void ) {
	_list	*history = g_terminal.currentTTY->history;
	_node	*ptr;

	ptr = history->first;

	for (size_t i = 0; i < history->size; i++) {
		VGA_PRINT(" - %s\n", ptr->ptr);
		ptr = ptr->next;
	}
}

void	resetData(_list	*history) {
	bzero(history->last->ptr, MAX_KEYBOARD_BUFFER);
	history->current = history->last;

	bzero(g_terminal.currentTTY->keyboardBuffer.buffer, g_terminal.currentTTY->keyboardBuffer.size);
	g_terminal.currentTTY->keyboardBuffer.size = 0;
	g_terminal.currentTTY->keyboardBuffer.index = 0;
}

void	addToHistory( void ) {
	_list	*history = g_terminal.currentTTY->history;

	if (!g_terminal.currentTTY->keyboardBuffer.buffer[0])
		return ;

	if (!strncmp((char *)g_terminal.currentTTY->keyboardBuffer.buffer, history->last->previous->ptr, MAX_KEYBOARD_BUFFER)) {
		resetData(history);
		return ;
	}

	safeStrcpy(history->last->ptr, g_terminal.currentTTY->keyboardBuffer.buffer, g_terminal.currentTTY->keyboardBuffer.size);

	history->last = history->last->next;

	resetData(history);

	if (history->first == history->last)
		history->first = history->first->next;
	else
		history->size++;
}
void	initHistory( void ) {
	_tty	*tty = g_terminal.currentTTY;
	_node	*ptr;
	uint8_t	index;

	index = tty->index;

	tty->history->first = &g_commandLine[index][0];
	ptr = tty->history->first;
	for (uint8_t i = 0; i < MAX_HISTORY; i++) {
		ptr->ptr = &g_historyBuffers[index][i];
		ptr->next = &g_commandLine[index][(i + 1) % MAX_HISTORY];
		if (i)
			ptr->previous = &g_commandLine[index][i - 1];
		ptr = ptr->next;
	}
	tty->history->first->previous = &g_commandLine[index][MAX_HISTORY - 1];;
	tty->history->last = tty->history->first;
	tty->history->current = tty->history->first;

	g_terminal.currentTTY->history->size = 0;

	SERIAL_SUCC("TTYs %d History Initialized", index);
}