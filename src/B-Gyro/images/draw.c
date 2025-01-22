# include "images/image.h"
# include "drivers/vga.h"
# include "terminal/vga.h"



void	drawCharacter(_vgaCell cell, size_t x, size_t y){
	uint16_t	shift;

	if (!CURRENT_TTY->mode->putPixel)
		return;

	for (size_t i = 0; i < FONT_HEIGHT; i++){
		shift = 1 << (FONT_WIDTH - 1);
		for (size_t j = 0; j < FONT_WIDTH; j++){
			if (CURRENT_TTY->font->pixels[((int32_t)(cell.character))][i] & shift)
				CURRENT_TTY->mode->putPixel((_positionPair){x + j, y + i}, cell.color & FOREGROUND_BITS);
			else
				CURRENT_TTY->mode->putPixel((_positionPair){x + j, y + i}, (cell.color & BACKGROUND_BITS) >> 4);
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
// void	drawCharacters(void) {
// 	if (!CURRENT_TTY->mode || !CURRENT_TTY->mode->putPixel)
// 		return;
// 	size_t x, y;

// 	x = 0;
// 	y = 0;

// 	for (size_t c = 0; c < 256 ; c++) {
// 		drawCharacter(c, x, y);
// 		x += 9;
// 		if ((x >= (71 * 8))) {
// 			y += 16;
// 			x = 0;
// 		}
// 	}
// }

// to do: only temporary
void	drawCursor(_image *image, size_t x, size_t y){
	uint8_t	color;
	
	if (!CURRENT_TTY->mode->putPixel)
		return;
	
	for (size_t i = 0; i < image->height; i++){
		for (size_t j = 0; j < image->width; j++){
			if (image->pixels[i][j] == 'B') 
				color = BODER_COLOR;
			else if (image->pixels[i][j] == 'F')
				color = FILL_COLOR;
			else if (DRAW_SHADOW && (image->pixels[i][j] == 'S'))
				color = SHADOW_COLOR;
			else
				continue;
			CURRENT_TTY->mode->putPixel((_positionPair){x + j, y + i}, color);
		}
	}
}


// to do: only temporary
void	drawCharsImages(_font *image, size_t x, size_t y){
	// uint8_t	color;
	
	if (!CURRENT_TTY->mode->putPixel)
		return;
	
	for (size_t i = 0; i < image->height; i++){
		for (size_t j = 0; j < image->width; j++){
			CURRENT_TTY->mode->putPixel((_positionPair){x + j, y + i}, image->pixels[i][j]);
		}
	}
}

// void	drawImage(_image *image, size_t x, size_t y){
// 	uint8_t	color;
	
// 	if (!CURRENT_TTY->mode->putPixel)
// 		return;
	
// 	for (size_t i = 0; i < image->height; i++){
// 		for (size_t j = 0; j < image->width; j++){
// 			if (image->pixels[i][j] < 0) 
// 				CURRENT_TTY->mode->putPixel((_positionPair){x + j, y + i}, image->pixels[i][j]);
// 		}
// 	}
// }



