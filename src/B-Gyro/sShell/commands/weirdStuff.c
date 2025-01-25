# include "drivers/vga.h"
# include "klibc/print.h"
# include "klibc/memory.h"
# include "arch/i386/ports/portsIO.h"

void drawSquareBkolchiLih(_positionPair pos, size_t lenght, uint8_t color, putPixelFct putPixel) {
    for (size_t y = pos.y; y < lenght; y++) {
        for (size_t x = pos.x; x < lenght; x++) {
			putPixel((_positionPair){x, y}, color);
        }
    }
}


void	clearScreen(_vgaMode *vgaMode){
	for (size_t i = 0; i < vgaMode->screenHeight; i++)
	{
		for (size_t j = 0; j < vgaMode->screenWidth; j++){
			_positionPair pos = {j, i};
			vgaMode->putPixel(pos, 0);
		}
	}
}

static void sleep(void){
	for (size_t i = 0; i < 10000; i++){
		for (size_t j = 0; j < 10000; j++){}
	}
}

void	drawSquare(char *args){
	(void)args;
	//_vgaMode *vgaMode = changeVGAMode13h();
	//_vgaMode *vgaMode = changeVGAMode640x480x2();
	changeVGAMode640x480x16();

	clearScreen(CURRENT_TTY->mode);

	uint8_t i = 0;
	_positionPair pos = {.x = 0, .y = 0};
	while (1) {
		drawSquareBkolchiLih(pos, 50, i, CURRENT_TTY->mode->putPixel);
		i++;
		if (i >= 5)
			while (1);
		sleep();
	}

	changeVGAModeT80x25();
}