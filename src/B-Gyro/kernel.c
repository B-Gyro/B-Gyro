# include "klibc/print.h"
# include "arch/i386/cpu/descriptorTables.h"

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

void	kernelInits(void){
	testGDT();
	initDescriptorTables();
	testGDT();
	SERIAL_SUCC("Descriptor Tables Inits Done");
	SERIAL_SUCC("Kernel Inits Done");
}

int	kmain(void){

	kernelInits();

	SERIAL_PRINT("Hello %s\n", "World!");
	return 0;
}