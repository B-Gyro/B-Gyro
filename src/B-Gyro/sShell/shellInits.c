# include "terminal/tty.h"
# include "sshell/sshell.h"
# include "klibc/strings.h"
# include "drivers/keyboard.h"

_command g_sshelCommands[MAX_COMMANDS];


void	sshellAddCommand(char *name, commandFunc func){
	static uint8_t	index;
	size_t			nameLen;

	if (index >= MAX_COMMANDS) {
		SERIAL_ERR("MAX_COMMANDS ? it was named like that for a reason :)");
		return ;
	}
	nameLen = strlen(name);
	if (nameLen > MAX_COMMAND_NAME) {
		SERIAL_ERR("huh ? '%s' ... too long for a command name, don't you think so ?", name);
		return ;
	}
	strlcpy(g_sshelCommands[index].name, name, nameLen);
	g_sshelCommands[index].func = func;
	index++;
}

void	sshellInitCommands(){
	sshellAddCommand("clear", sshelClear);
    //sshellAddCommand("peek", peek);
    //sshellAddCommand("poke", poke);
    //sshellAddCommand("hlt", hltCmd);
    //sshellAddCommand("dump", dumpCmd);
    //sshellAddCommand("reboot", reboot);
    //sshellAddCommand("clear", clearTty);
    //sshellAddCommand("stack", printStack);
    //sshellAddCommand("help", listCommands);
}

bool	sshellExecCommand(char *buffer){
	char *name = strtok(buffer, " ");
    char *args = strtok(NULL, NULL);

    for (uint8_t i = 0; i < MAX_COMMANDS; i++){
        if (!strcmp(g_sshelCommands[i].name, name)){
            if (g_sshelCommands[i].func)
                g_sshelCommands[i].func(args);
            return 0;
        }
    }
    VGA_PRINT("%s: Command not found\n", name);
    return 1;
}

void	sshellInit(void){
	sshellInitCommands();
}

void	sshellStart(void){
	char	buffer[256];
	sshellInit();

	while (1){
		prompt("$", buffer);
        if (!(*buffer))
            continue;
		sshellExecCommand(buffer);
	}
}
