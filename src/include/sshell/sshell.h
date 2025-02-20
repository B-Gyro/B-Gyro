# pragma once

# define MAX_COMMANDS 24
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
void	shutdown(char *args);
void	history(char *args);
void    logout(char *args);
void	whoami(char *args);
void	adduser(char *args);
void	deluser(char *args);
void	su(char *args);
void	lspci(char *args);
void	poke(char *args);
void	peek(char *args);
void	time(char *args);
void	date(char *args);
void	datetime(char *args);
void	timer(char *args);
void	screentime(char *args);

void	help(char *args);
// additionals:
void	visualStuff(char *args);
// void	drawSquare(char *args);

void	sshellStart(void);


// shortcuts
void	altC(void);
void	ctrlC(void);
void	ctrlD(void);