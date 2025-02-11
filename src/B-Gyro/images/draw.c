# include "images/image.h"
# include "drivers/vga.h"
# include "terminal/vga.h"

void	draw8Pixels640x480x16(_positionPair pos, uint8_t byte, uint8_t forgroundColor, uint8_t backgroundColor);

void	drawCharacterBy8Pixels(_vgaCell cell, size_t x, size_t y){
	uint8_t		foreground, background;

	foreground = cell.color & FOREGROUND_BITS;
	background = (cell.color & BACKGROUND_BITS) >> 4;
	if (!CURRENT_TTY->mode->putPixel)
		return;
	for (size_t i = 0; i < FONT_HEIGHT; i++)
		draw8Pixels640x480x16((_positionPair){x, y + i}, CURRENT_TTY->font->pixels[((int32_t)(cell.character))][i], foreground, background);
}

void	drawCharacterByPixel(_vgaCell cell, size_t x, size_t y){
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



