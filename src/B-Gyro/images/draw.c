# include "images/image.h"
# include "drivers/vga.h"
# include "terminal/vga.h"

void	drawCharacter(_vgaCell cell, size_t x, size_t y){
	uint16_t	shift;
	uint8_t		foreground, background;

	foreground = cell.color & FOREGROUND_BITS;
	background = (cell.color & BACKGROUND_BITS) >> 4;


	if (!CURRENT_TTY->mode->putPixel)
		return;
	for (size_t i = 0; i < FONT_HEIGHT; i++){
		shift = 1 << (FONT_WIDTH - 1);
		for (size_t j = 0; j < FONT_WIDTH; j++){
			if (CURRENT_TTY->font->pixels[((int32_t)(cell.character))][i] & shift)
				CURRENT_TTY->mode->putPixel((_positionPair){x + j, y + i}, foreground);
			else
				CURRENT_TTY->mode->putPixel((_positionPair){x + j, y + i}, background);
			shift >>= 1;
		}
	}
}

void	drawFilledRectangle(size_t x, size_t y, size_t width, size_t height, uint16_t color){
	for (size_t i = 0; i < height; i++){
		for (size_t j = 0; j < width; j++)
			CURRENT_TTY->mode->putPixel((_positionPair){x + j, y + i}, color);
	}
}

void	drawImage(_image *image, size_t x, size_t y){
	if (!(CURRENT_TTY->mode->putPixel))
		return;
	
	for (size_t i = 0; i < image->height; i++){
		for (size_t j = 0; j < image->width; j++){
			if (image->pixels[i][j] > -1) 
				CURRENT_TTY->mode->putPixel((_positionPair){x + j, y + i}, image->pixels[i][j]);
		}
	}
}



