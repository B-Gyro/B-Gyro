# include "drivers/vga.h"
# include "klibc/print.h"
# include "terminal/vga.h"

static void	putPixel(_positionPair pos, uint8_t color);
static void clearVGA320x200x256(bool clearFull);

_vgaMode g_G320x200x256 = {
	.func = changeVGAMode13h,
	.putCharPos = NULL,
	.putPixel = putPixel,
	.screenHeight = 200,
	.screenWidth = 320,
	.VMStart = (char *)0xA0000,
	.clearScreen = clearVGA320x200x256,
	.maxColors = 256
};

static void	putPixel(_positionPair pos, uint8_t color){
	char	*videoMemory = g_G320x200x256.VMStart;

	videoMemory[pos.x + pos.y * g_G320x200x256.screenWidth] = color;
}

static void clearVGA320x200x256(bool clearFull) {
	for (size_t i = 0; i < CURRENT_TTY->mode->screenHeight - !clearFull * FONT_HEIGHT; i++){
		for (size_t j = 0; j < CURRENT_TTY->mode->screenWidth; j++)
			CURRENT_TTY->mode->putPixel((_positionPair){j, i}, DEFAULT_BACKGROUND_COLOR);
	}
	CURRENT_TTY->posX = 0;
	CURRENT_TTY->posY = 0;
}

void	changeVGAMode13h(void){
	uint8_t G320x200x256[] = {
		/* MISC */
		0x63,
		/* SEQ */
		0x03, 0x01, 0x0F, 0x00, 0x0E,
		/* CRTC */
		0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
		0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
		0xFF,
		/* GC */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
		0xFF,
		/* AC */
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		0x41, 0x00, 0x0F, 0x00,	0x00
	};
	dumpToVGAPorts(G320x200x256);

	CURRENT_TTY->mode = &g_G320x200x256;
	CURRENT_TTY->font = &g_font8x8;
	setDefaultPalette();
	clearVGA320x200x256(1);
}
