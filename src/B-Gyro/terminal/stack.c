#include "klibc/print.h"
#include "klibc/memory.h"
#include "terminal/tty.h"
#include "sshell/sshell.h"

extern void	*start; 

# define STACK_START_PTR(address)	{address = (uintptr_t) &start;}
# define STACK_CURRENT_PTR(address)	{__asm__ volatile ("mov %%esp, %0" : "=r" (address));}

uint32_t	stackUsedMemory( void ){
	uintptr_t	startStackPtr;
	uintptr_t	currentStackPtr;

	STACK_START_PTR(startStackPtr);
	STACK_CURRENT_PTR(currentStackPtr);
	// __asm__ volatile ("mov %%esp, %0" : "=r" (currentStackPtr));  

	print(serialPutChar, " + %db\n",  currentStackPtr - startStackPtr);
	print(serialPutChar, " + %dKb\n", (currentStackPtr - startStackPtr) / 1024);
	print(serialPutChar, " + %dMb\n", (currentStackPtr - startStackPtr) / (1024 * 1024));

	return (startStackPtr - currentStackPtr);
}
