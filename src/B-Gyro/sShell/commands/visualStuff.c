#include "klibc/memory.h"
#include "klibc/print.h"
#include "drivers/keyboard.h"
#include "drivers/vga.h"

extern _vgaMode g_G320x200x256;

void	drawCircle(uint32_t x, uint32_t y, uint16_t radius, uint8_t color){
	(void)x;
	(void)y;
	(void)radius;
	(void)color;

	float p = 1;

	uint32_t x0 = 0, y0 = radius;
	while (y0 >= x0){
		g_G320x200x256.putPixel((_positionPair){.x = x + x0, .y = y + y0}, color);
		// drawLine((_positionPair){.x = x, .y = y},(_positionPair){.x = x + x0, .y = y + y0}, color);

    	g_G320x200x256.putPixel((_positionPair){.x = x + x0, .y = y - y0}, color);
		// drawLine((_positionPair){.x = x, .y = y},(_positionPair){.x = x + x0, .y = y - y0}, color);

    	g_G320x200x256.putPixel((_positionPair){.x = x - x0, .y = y - y0}, color);
		// drawLine((_positionPair){.x = x, .y = y},(_positionPair){.x = x - x0, .y = y - y0}, color);
		
		g_G320x200x256.putPixel((_positionPair){.x = x - x0, .y = y + y0}, color);
		// drawLine((_positionPair){.x = x, .y = y}, (_positionPair){.x = x - x0, .y = y + y0}, color);

    	g_G320x200x256.putPixel((_positionPair){.x = x + y0, .y = y + x0}, color);
		// drawLine((_positionPair){.x = x, .y = y},(_positionPair){.x = x + y0, .y = y + x0}, color);

    	g_G320x200x256.putPixel((_positionPair){.x = x - y0, .y = y + x0}, color);
		// drawLine((_positionPair){.x = x, .y = y},(_positionPair){.x = x - y0, .y = y + x0}, color);

    	g_G320x200x256.putPixel((_positionPair){.x = x + y0, .y = y - x0}, color);
		// drawLine((_positionPair){.x = x, .y = y},(_positionPair){.x = x + y0, .y = y - x0}, color);

    	g_G320x200x256.putPixel((_positionPair){.x = x - y0, .y = y - x0}, color);
		// drawLine((_positionPair){.x = x, .y = y},(_positionPair){.x = x - y0, .y = y - x0}, color);


		x0++;
		if (p > 0)
			y0--;
		// p = 2 * ((x0 * x0) + (y0 * y0) - y0 - radius) + 1;
		// p = 2 * ((x0 * x0) + (y0 * y0) - radius) - y0;
		p = x0 * x0 + y0 * y0 - y0 + 0.25 - radius * radius;
	}
}

void	visualStuff(char *args){

	(void) args;
	changeVGAMode13h();
	asm volatile("cli");

	drawCircle(100, 100, 50, 14);
	
	while (1);
	changeVGAMode640x480x16();
}