#include "klibc/print.h"
#include "terminal/terminal.h"
#include "arch/i386/cpu/descriptorTables.h"

// always call initTTY(0); before starting to work with terminal

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

void	timerHandler(_registers r){
	static uint32_t tick = 0;

	(void)r;
	tick++;
	if (tick % 100 == 0)
		SERIAL_INFO("Tick: %d", tick);
}

void	keyboardHandler(_registers r){
	uint8_t scancode;

	(void)r;
	scancode = portByteIn(0x60);
	SERIAL_INFO("Scancode: %d", scancode);
}

void	initIRQHandlers(){
	SERIAL_INFO("Initializing IRQ Handlers");
	setIRQHandler(0, timerHandler);
	setIRQHandler(1, keyboardHandler);
	SERIAL_SUCC("IRQ Handlers Initialized");
}


void	kernelInits(void){
	testGDT();
	initDescriptorTables();
	testGDT();
	SERIAL_SUCC("Descriptor Tables Initialized");
	initIRQHandlers();
	g_terminal.currentTTY->index = 0;
	initTTY(0);
	SERIAL_SUCC("Kernel Initialized");
}


int	kmain(void){

	kernelInits();

	VGA_PRINT("\033[35mHello %s", "\033[37;102mWorld\033[0m!");
	while (1);
  
	return 0;
}
