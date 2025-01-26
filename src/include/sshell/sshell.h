# pragma once

# define MAX_COMMANDS 16
# define MAX_COMMAND_NAME 32

# define AVAILABLE_COMMANDS 10


typedef void (*commandFunc)(char *arg);

typedef struct command
{
    char		name[MAX_COMMAND_NAME];
    commandFunc	func;
} _command;


//// commands:
void	clear(char *args);
void    reboot(char *args);
void	history(char *args);
void    logout(char *args);
void	whoami(char *args);
void	adduser(char *args);
void	deluser(char *args);
void	su(char *args);
void	lspci(char *args);
void	poke(char *args);
void	peek(char *args);

void	help(char *args);
// additionals:
void	drawSquare(char *args);
void	initCub3d(char *args);

void	sshellStart(void);


// shortcuts
void	altC(void);
void	ctrlC(void);
void	ctrlD(void);