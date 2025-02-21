#include "drivers/vga.h"
#include "klibc/print.h"
#include "klibc/memory.h"
#include "arch/i386/ports/portsIO.h"
# include "terminal/vga.h"

void clearTextMode(bool clearFull);

_vgaMode g_T80x25 = {
	.func = changeVGAModeT80x25,
	.putCharPos = putCharPos,
	.putPixel = NULL,
	.screenHeight = 25,
	.screenWidth = 80,
	.VMStart = (char *)0xB8000,
	.clearScreen = clearTextMode,
	.maxColors = 16
};

void clearTextMode(bool clearFull) {
	// _vgaCell *adress = (_vgaCell *)VIDEO_ADDRESS;
	// _vgaCell cell;

	// cell.character = ' ';
	// cell.color = DEFAULT_BACKGROUND_COLOR << 4;

	// for (size_t i = 0; i < CURRENT_TTY->mode->screenHeight - !clearFull; i++){
	// 	for (size_t j = 0; j < CURRENT_TTY->mode->screenWidth; j++)
	// 		adress[i * MAX_COLUMNS + j] = cell;
	// }
	bigBzero((uint16_t *)VIDEO_ADDRESS, (MAX_ROWS + clearFull) * MAX_COLUMNS);
	
	CURRENT_TTY->posX = 0;
	CURRENT_TTY->posY = 0;
}

void	changeVGAModeT80x25(void){
	uint8_t T80x25[] =
	{
	/* MISC */
		0x67,
	/* SEQ */
		0x03, 0x00, 0x03, 0x00, 0x02,
	/* CRTC */
		0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F,
		0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x50,
		0x9C, 0x0E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3,
		0xFF,
	/* GC */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
		0xFF,
	/* AC */
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
		0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
		0x0C, 0x00, 0x0F, 0x08, 0x00
	};
	if (CURRENT_TTY->mode == &g_T80x25)
		return ;

	dumpToVGAPorts(T80x25);
	setFont(g_8x16_font, 16);
	
	CURRENT_TTY->mode = &g_T80x25;
	CURRENT_TTY->font = &g_fontText;
	clearTextMode(1);
}