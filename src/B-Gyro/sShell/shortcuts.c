#include "drivers/keyboard.h"
#include "klibc/print.h"

void	ctrlC(void){
	keyboardClearBuffer();
	inturruptPrompting();
	SERIAL_SUCC("CTRL+C PRESSED");
}

void	ctrlD(void) {
	inturruptPrompting();
	SERIAL_SUCC("CTRL+D PRESSED");
}

void	altC(void){
	SERIAL_SUCC("ALT+C pressed");
}