#include "drivers/vga.h"
#include "klibc/print.h"
#include "arch/i386/ports/portsIO.h"


static void	putPixel(_positionPair pos, uint8_t color);

_vgaMode g_G640x480x2 = {
	.func = changeVGAMode640x480x2,
	.putCharPos = NULL,
	.putPixel = putPixel,
	.screenHeight = 480,
	.screenWidth = 640,
	.VMStart = (char *)0xA0000,
	.maxColors = 2
};

static void putPixel(_positionPair pos, uint8_t color) {
	size_t offset = (pos.y * (g_G640x480x2.screenWidth / 8)) + (pos.x / 8);
	uint8_t	mask = 0b10000000 >> (pos.x % 8);
	if (color)
		g_G640x480x2.VMStart[offset] |= mask;
	else
		g_G640x480x2.VMStart[offset] &= ~mask;
}

void	changeVGAMode640x480x2(void){
	uint8_t G640x480x2[] =
	{
	/* MISC */
		0xE3,
	/* SEQ */
		0x03, 0x01, 0x0F, 0x00, 0x06,
	/* CRTC */
		0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
		0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
		0xFF,
	/* GC */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0F,
		0xFF,
	/* AC */
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
		0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
		0x01, 0x00, 0x0F, 0x00, 0x00
	};
	if (CURRENT_TTY->mode == &g_G640x480x2)
		return ;
	dumpToVGAPorts(G640x480x2);
	CURRENT_TTY->mode = &g_G640x480x2;
	CURRENT_TTY->font = &g_font8x16;
}