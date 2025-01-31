#include "klibc/types.h"
#include "klibc/strings.h"
#include "klibc/print.h"
#include "klibc/memory.h"
#include "terminal/tty.h"

# define BUFFER_SIZE 1000

char	buffer[BUFFER_SIZE];
size_t	index = 0;
size_t	lines = 0;
size_t	len = 0; // for now buffer always starts with \n\r so len always starts with 0


static void	scroll(uint8_t n){
	_list *buffer;

	buffer = CURRENT_TTY->buffer;
	for (uint8_t i = 0; i < n; i++) {
		buffer->first = buffer->first->next;
		CURRENT_TTY->cursorY--;
		buffer->size--;
	}

	putTtyBuffer();
}

void	emptyBuffer( void ){
	size_t	diff;

	if (!*buffer)
		return;
	if (lines > 1) {
		diff = MAX_ROWS - (CURRENT_TTY->posY + 1);
		if (diff < lines)
			scroll (lines - diff);
	}
	VGA_PRINT("%s", buffer);
	index = 0;
	lines = 0;
	len = 0;
	bigBzero(buffer, BUFFER_SIZE / 2);
}

uint8_t	putCharToBuffer(char c){
	buffer[index++] = c;
	len++;

	if ((index == BUFFER_SIZE) || (lines >= (MAX_ROWS - 1))){
		PRINT_BUFFER();
		return (0);
	}

	if ((c == '\n') || (len > MAX_COLUMNS)){
		len = 0;
		lines++;
	}

	return (index);
}