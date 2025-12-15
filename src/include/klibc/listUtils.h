#pragma once

# include "klibc/types.h"
# include "klibc/strings.h"
# include "klibc/memory.h"
# include "klibc/print.h"
# include "terminal/_data.h"

// # define DEC_BASE	10
// # define HEXA_BASE	16

# define GREY_ANSI		"\033[38;5;238m"
# define MAGENTA_ANSI	"\033[1;35m"
# define RED_ANSI		"\033[0;31m"
# define GREEN_ANSI		"\033[0;32m"
# define RESET_ANSI		"\033[0;0m"

typedef struct ptr
{
	struct ptr *next;
	struct ptr *previous;	
} _ptr;

// void	printList(_ptr **list, short offset);
void	insertNode(_ptr **list, _ptr *node);
void	deleteNode(_ptr **list, _ptr *node);


//  >

typedef struct list _list;
typedef struct node _node;

void	insertNodeInList(_list *list, _node *node);
void	makeItCircularList(_list *list);