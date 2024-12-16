# pragma once

# define MAX_COMMANDS 16
# define MAX_COMMAND_NAME 32

typedef void (*commandFunc)(char *arg);

typedef struct command
{
    char		name[MAX_COMMAND_NAME];
    commandFunc	func;
} _command;

//// commands:
//void    poke(char *args);
//void    echo(char *args);
//void    peek(char *args);
//void    reboot(char *args);
//void    haltCmd(char *args);
//void    dumpCmd(char *args);
//void    clearTty(char *args);
//void    printStack(char *args);


void	sshellStart(void);