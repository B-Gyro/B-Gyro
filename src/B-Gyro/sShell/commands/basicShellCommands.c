# include "klibc/print.h"
# include "klibc/memory.h"
# include "terminal/tty.h"
# include "sshell/sshell.h"
# include "klibc/strings.h"


void	clear(char *args) {
	char *firstArgument;

	firstArgument = strtok(args, " ");
	if (strncmp(firstArgument, "-h", 2) == 0) {
		VGA_PRINT("Usage: bro ? you don't know what clear does ? try it !!!\n");
		return;
	}
	clearTTY(SCREEN_SIZE);
}

void	history(char *args) {
	char *firstArgument;

	firstArgument = strtok(args, " ");
	if (strncmp(firstArgument, "-h", 2) == 0) {
		VGA_PRINT("Usage: bro ? you don't know what clear does ? try it !!!\n");
		return;
	}
	printHistory();
}