# include "klibc/print.h"
# include "klibc/memory.h"
# include "terminal/tty.h"
# include "sshell/sshell.h"
# include "klibc/strings.h"

extern _command g_sshelCommands[MAX_COMMANDS];
extern uint8_t	g_availableCommands;
extern size_t	g_ticks;

void	clear(char *args) {
	char *arg;

	arg = strtok(args, " ");
	if (!strncmp(arg, "-h", 2) || !strncmp(arg, "--help", 6)) {
		FILL_BUFFER("Clears the terminal screen.\n");
		return;
	}	
	clearTTY(SCREEN_SIZE);
}

void	history(char *args) {
	char *arg;

	arg = strtok(args, " ");
	if (!strncmp(arg, "-h", 2) || !strncmp(arg, "--help", 6)) {
		FILL_BUFFER("Displays the last %d history of commands for the current session.\n", MAX_HISTORY);
		return;
	}
	printHistory();
}


void	help(char *args) {
	char	*arg;

	arg = strtok(args, " ");
	if (arg) {
		if (!strncmp(arg, "-h", 2) || !strncmp(arg, "--help", 6)) {
			FILL_BUFFER("Are you serious right now !!\n");
			return ;
		}
	}

	for (uint8_t i = 0; i < g_availableCommands - 1; i++){
		FILL_BUFFER("%s%s: \033[0m\n", COLOR_CYAN, g_sshelCommands[i].name);
		//g_sshelCommands[i].func("-h");
	}
}

void	whoami(char *args) {
	if (args && *args) {
		FILL_BUFFER("%sInvalid options `%s`\n%s", COLOR_RED, args, COLOR_RESET);
		return;
	}
	FILL_BUFFER("%s\n", CURRENT_USER->username);
}