# include "klibc/print.h"
# include "klibc/memory.h"
# include "terminal/tty.h"
# include "klibc/converts.h"
# include "sshell/sshell.h"
# include "klibc/strings.h"
# include "drivers/vga.h"

void poke(char *args){
	char		*arg;
	uintptr_t	addr;
	uint32_t	index;
	uint8_t		value;

	arg = strtok(args, " ");

	if (!strncmp(arg, "-h", 2)){
		FILL_BUFFER("format: poke addr value, usage: put value[8bit, dec] in addr[32bit, hex]\n");
		return ;
	}
	addr = aHextoiS(arg, NULL);
	if (!addr)
		return printError("incorrect Address, USE -h for more info");

	arg = strtok(NULL, " ");
	value = atoiS(arg, &index);
	if (!index)
		return printError("incorrect value, USE -h for more info");

	*(uint8_t *)addr = value;
}

void peek(char *args){
	char		*arg;
	uint8_t		*addr;

	arg = strtok(args, " ");

	if (!strncmp(arg, "-h", 2)){
		FILL_BUFFER("format: peek addr usage: get value in addr[32bit hex]\n");
		return ;
	}
	addr = (uint8_t *)aHextoiS(arg, NULL);
	if (!addr)
		return printError("incorrect Address, USE -h for more info");

	FILL_BUFFER("%08p: BIN[%08b], DEC[%0d], HEX[%02x], ASSCI[%c]\n", addr, *addr, *addr, *addr, *addr);
}

void	dump(char *args){
	char *arg;
	uint8_t	*startAddr;
	ssize_t	size;
	
	arg = strtok(args, " ");
	VGA_PRINT("\n");
	if (!strncmp(arg, "-h", 2)){
		FILL_BUFFER("format: peek addr usage: get value in addr[32bit hex]\n");
		return ;
	}
	startAddr = (uint8_t *)aHextoiS(arg, NULL);
	if (!startAddr)
		return printError("incorrect Address, USE -h for more info");
	
	arg = strtok(NULL, " ");
	if (!arg)
		return printError("size ??, USE -h for more info");

	size = atoiS(arg, NULL);
	if (size <= 0)
		return printError("size ??, USE -h for more info");

	dumpMemory(startAddr, size);
}

extern void	*__stack_top;
# define STACK_START_PTR(address)	{address = (uint8_t *) &__stack_top;}
# define STACK_CURRENT_PTR(address)	{__asm__ volatile ("mov %%esp, %0" : "=r" (address));}

void	dumpStack(char *args){
	uint8_t	*startStackPtr;
	uint8_t	*currentStackPtr;

	STACK_START_PTR(startStackPtr);
	STACK_CURRENT_PTR(currentStackPtr);
	
	(void)args;
	VGA_PRINT("\n");
	dumpMemory(currentStackPtr, startStackPtr - currentStackPtr);
}