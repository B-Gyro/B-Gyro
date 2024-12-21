#include "drivers/keyboard.h"
#include "klibc/print.h"

void	ctrlC(void){
	keyboardClearBuffer();
	interruptPrompting();
	SERIAL_SUCC("CTRL+C PRESSED");
}

void	ctrlD(void) {
	interruptPrompting();
	SERIAL_SUCC("CTRL+D PRESSED");
}

void	altC(void){
	SERIAL_SUCC("ALT+C pressed");
}