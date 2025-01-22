#include "drivers/vga.h"
#include "klibc/print.h"
#include "arch/i386/ports/portsIO.h"


static void	putPixel(_positionPair pos, uint8_t color);

_vgaMode g_G640x480x16 = {
	.func = changeVGAMode640x480x16,
	.putCharPos = NULL,
	.putPixel = putPixel,
	.screenHeight = 480,
	.screenWidth = 640,
	.VMStart = (char *)0xA0000,
	.maxColors = 16
};
static void putPixel(_positionPair pos, uint8_t color)
{
    size_t offset = (80 * pos.y) + (pos.x / 8);
    uint8_t mask = 0x80 >> (pos.x % 8);

	// only effect 1 bit at the time:
	portByteOut(GRAPHICS_REG_ADDR, BIT_MASK_REG);
    portByteOut(GRAPHICS_REG_DATA, mask);

	for (uint8_t plane = 0; plane < 4; plane++) {
        setVideoPlane(plane);
        if (color & (1 << plane))
            g_G640x480x16.VMStart[offset] |= mask;
        else
            g_G640x480x16.VMStart[offset] &= ~mask;
    }
}

void	changeVGAMode640x480x16(void){
	uint8_t G640x480x16[] = {
	/* MISC */
		0xE3,
	/* SEQ */
		0x03, 0x01, 0x08, 0x00, 0x06,
	/* CRTC */
		0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
		0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
		0xFF,
	/* GC */
		0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
		0xFF,
	/* AC */
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
		0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
		0x01, 0x00, 0x0F, 0x00, 0x00
	};
	dumpToVGAPorts(G640x480x16);
	CURRENT_TTY->mode = &g_G640x480x16;
	CURRENT_TTY->font = &g_font8x16;
}