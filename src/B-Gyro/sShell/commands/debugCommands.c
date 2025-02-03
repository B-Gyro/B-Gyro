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

