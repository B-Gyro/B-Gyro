#include "terminal/_data.h"
#include "klibc/memory.h"

void	printHistory( void ) {
	_list	*history = g_terminal.currentTTY->history;
	_node	*ptr;

	ptr = history->first;

	for (size_t i = 0; i < history->size; i++) {
		VGA_PRINT(" - %s\n", ptr->ptr);
		ptr = ptr->next;
	}
}

void	addToHistory( void ) {
	_list	*history = g_terminal.currentTTY->history;
	_node	*first, *last;

	if (!g_terminal.currentTTY->keyboardBuffer.buffer[0])
		return ;

	first = history->first;
	last = history->last;

	safeStrcpy(last->ptr, g_terminal.currentTTY->keyboardBuffer.buffer, MAX_KEYBOARD_BUFFER);

	history->last = last->next;
	bzero(last->next->ptr, MAX_KEYBOARD_BUFFER);

	if (first == last->next)
		history->first = first->next;
	else
		history->size++;
}

void	initHistory( void ) {
	_tty	*tty = g_terminal.currentTTY;
	_node	*ptr;
	uint8_t	i, index;

	index = tty->index;

	tty->history->first = &g_commandLine[index][0];
	ptr = tty->history->first;
	for (i = 0; i < (MAX_HISTORY - 1); i++) {
		ptr->ptr = &g_historyBuffers[index][i];
		ptr->next = &g_commandLine[index][i + 1];
		if (i)
			ptr->previous = &g_commandLine[index][i - 1];
		ptr = ptr->next;
	}
	ptr->ptr = &g_historyBuffers[index][i];
	ptr->next = tty->history->first;
	((_node *)tty->history->first)->previous = &g_commandLine[index][MAX_HISTORY - 1];

	tty->history->last = tty->history->first;

	g_terminal.currentTTY->history->size = 0;

	SERIAL_SUCC("TTYs %d History Initialized", index);
}
