#include "klibc/memory.h"
#include "klibc/print.h"
#include "drivers/keyboard.h"
#include "drivers/vga.h"

extern _vgaMode g_G320x200x256;

void	drawCircle(uint32_t x, uint32_t y, uint16_t radius, uint8_t color){
	
}

void	visualStuff(char *args){

	(void) args;
	changeVGAMode13h();
	asm volatile("cli");

	drawCircle(0, 0, 10, 14);
	
	while (1);
	changeVGAMode640x480x16();
}