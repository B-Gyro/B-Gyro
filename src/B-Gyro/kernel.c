# include "klibc/print.h"

int	kmain(void){

	SERIAL_PRINT("Hello %s\n", "World!");
	return 0;
}