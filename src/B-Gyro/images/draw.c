# include "images/image.h"
# include "drivers/vga.h"


void	drawCharacter(uint8_t character, size_t x, size_t y){
	uint16_t	shift;

	if (!g_vgaMode || !g_vgaMode->putPixel)
		return;

	for (size_t i = 0; i < (g_font)->height; i++){
		shift = 1 << ((g_font)->width - 1);
		for (size_t j = 0; j < (g_font)->width; j++){
			if ((g_font)->pixels[character][i] & shift)
				g_vgaMode->putPixel((_positionPair){x + j, y + i}, g_currentTextColor);
			else
				g_vgaMode->putPixel((_positionPair){x + j, y + i}, g_currentBackGroundColor);
			shift >>= 1;
		}
	}
}

void	drawCharacters(void) {
	if (!g_vgaMode || !g_vgaMode->putPixel)
		return;
	size_t x, y;

	x = 0;
	y = 0;

	for (size_t c = 0; c < 256 ; c++) {
		drawCharacter(c, x, y);
		x += 9;
		if ((x >= (71 * 8))) {
			y += 16;
			x = 0;
		}
	}
}

// to do: only temporary
void	drawCursor(_image *image, size_t x, size_t y){
	uint8_t	color;
	
	if (!g_vgaMode->putPixel)
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
			g_vgaMode->putPixel((_positionPair){x + j, y + i}, color);
		}
	}
}

// void	drawImage(_image *image, size_t x, size_t y){
// 	uint8_t	color;
	
// 	if (!g_vgaMode->putPixel)
// 		return;
	
// 	for (size_t i = 0; i < image->height; i++){
// 		for (size_t j = 0; j < image->width; j++){
// 			if (image->pixels[i][j] < 0) 
// 				g_vgaMode->putPixel((_positionPair){x + j, y + i}, image->pixels[i][j]);
// 		}
// 	}
// }



