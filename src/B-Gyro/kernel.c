#include "klibc/print.h"
#include "terminal/terminal.h"
#include "arch/i386/cpu/descriptorTables.h"

void testGDT() {
	uint32_t	cr0;
	_gdtPtr		gdt = {.base = 0, .limit = 0};

	SERIAL_INFO("Testing GDT");	
	asm volatile("sgdt %0" : : "m" (gdt));
	SERIAL_DEBUG("GDT Base Address: %08p", gdt.base);
	SERIAL_DEBUG("GDT Limit: %p", gdt.limit);
	SERIAL_DEBUG("GDT Entries Count: %d", (gdt.limit + 1) / sizeof(_gdtEntry));
	/*Testing if paging Enabled*/
	asm volatile("mov %%cr0, %0" : "=r" (cr0));
	SERIAL_DEBUG("Paging is currently: %s", (cr0 & 0x80000000) ? "ENABLED": "DISABLED");
	SERIAL_INFO("GDT Test Done");
}

void	sleep(uint8_t n) {
	uint32_t X = 2500000;  // 6 * 10^8 for 1s

   	//SERIAL_PRINT("Hello %s\n", "World!");
	X *= n;
	for (uint32_t x = 0; x < X; x++);
}

void	kernelInits(void){
	testGDT();
	initDescriptorTables();
	testGDT();
	SERIAL_SUCC("Descriptor Tables Initialized");
	g_terminal.currentTTY->index = 0;
	initTTY();
	SERIAL_SUCC("Terminal Initialized");
	SERIAL_SUCC("Kernel Initialized");
}


int	kmain(void){
	kernelInits();

	VGA_PRINT("Hello %s\n", "World!");

	return 0;
}


//int kmain(void)
//{
//	size_t i;

//	kernelInits();
//	i = 1;
//	for (i = 1; i <= MAX_ROWS ; i++){	
//		// for (size_t i = 0; i < 79; i++)
//		{
//			putChar((i/10)+48);
//			putChar((i%10)+48);
//		}
//		// if (i != MAX_ROWS)
//			putChar('\n');
//	}
//	// while (1)
//	// {
//	// 	putChar('\n');
//		for (size_t j = 0; j < (79 * 2); j++)
//		{
//			putCar((j%10) + 48);
//			sleep (1);
//		}
//		putChar('\n');

//		for (i = 1; i <= MAX_ROWS ; i++){	
//			// for (size_t i = 0; i < 79; i++)
//			{
//				putChar((i/10)+48);
//				putChar((i%10)+48);
//				sleep (1);

//			}
//			// if (i != MAX_ROWS)
//				putChar('\n');
//		}
//	// 	i++;
//	// 	sleep (1);	

//	// }
	
//	return 0;
//}


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