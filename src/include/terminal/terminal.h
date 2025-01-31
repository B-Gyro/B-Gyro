#pragma once

# include "terminal/_data.h"

typedef struct tty _tty; 
typedef struct node _node; 

void	initTerminal(void);

uint8_t	putChar(char c);
uint8_t	putCharPos(char c, uint32_t x, uint32_t y);
uint8_t putStrPos(char *str, uint32_t x, uint32_t y);

void	incrementPositionX(void);
void	incrementPositionY(void);
void	decrementPositionX(void);
void	decrementPositionY(void);

void	loginScreen(bool alreadyPrompted);

void	initUsers(void);
_node	*getUserID(char *username);
uint8_t	checkUser(char *user, char *pass);