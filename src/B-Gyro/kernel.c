#include "arch/i386/ports/serialCommunication/serialIO.h"
#include "terminal/terminal.h"

void	sleep(uint8_t n) {
	uint32_t X = 2500000;  // 6 * 10^8 for 1s

	X *= n;
	for (uint32_t x = 0; x < X; x++);
}

int kmain(void)
{
	size_t i;
	g_terminal.currentTTY->index = 0;
	initTTY();

	i = 1;
	for (i = 1; i <= MAX_ROWS ; i++){	
		// for (size_t i = 0; i < 79; i++)
		{
			putChar((i/10)+48);
			putChar((i%10)+48);
		}
		// if (i != MAX_ROWS)
			putChar('\n');
	}
	// while (1)
	// {
	// 	putChar('\n');
		for (size_t j = 0; j < (79 * 2); j++)
		{
			putChar((j%10) + 48);
			sleep (1);
		}
		putChar('\n');

		for (i = 1; i <= MAX_ROWS ; i++){	
			// for (size_t i = 0; i < 79; i++)
			{
				putChar((i/10)+48);
				putChar((i%10)+48);
				sleep (1);

			}
			// if (i != MAX_ROWS)
				putChar('\n');
		}
	// 	i++;
	// 	sleep (1);	

	// }
	
	return 0;
}


// int kmain(void)
// {

// 	_node		*line;
// 	size_t		i;
// 	_vgaCell	*cells;


// 	// initialisation
// 	g_terminal.currentTTY->index = 0;
// 	initTTY();


// 	line = g_terminal.currentTTY->buffer->first;


// 	// fill & print buffer
// 	i = 1;
// 	while (1) {
// 		cells = line->buffer;

// 		cells[0].character = (i / 10) + 48;
// 		cells[0].color = 0x07;

// 		cells[1].character = (i % 10) + 48;
// 		cells[1].color = 0x07;

// 		cells[2].character = ' ';
// 		cells[2].color = 0x07;

// 		for (size_t j = 3; j < 20; j++)
// 		{
// 			cells[j].character = (j - 3) + 65;
// 			cells[j].color = 0x07;
// 		}

// 		cells[20].character = '\n';
// 		cells[20].color = 0x07;

// 		if (line == g_terminal.currentTTY->buffer->first && (g_terminal.currentTTY->bufferSize != 1))
// 			g_terminal.currentTTY->buffer->first = line->next;

// 		if (g_terminal.currentTTY->bufferSize < MAX_ROWS)
// 			(g_terminal.currentTTY->bufferSize)++;
// 		line = line->next;
		
// 		putTtyBuffer();
// 		sleep(2);
// 		i++;
// 	}

// 	return 0;
// }