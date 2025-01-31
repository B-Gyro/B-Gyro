#include "images/image.h"
#include "drivers/vga.h"
#include "klibc/memory.h"
#include "terminal/vga.h"


static int32_t	edges[480][2];

static void	storePixel(_positionPair start, uint8_t color, bool b){
	int32_t	x, y;
	
	(void)color;
	if (b) {
		x = start.y;
		y = start.x;
	}
	else {
		x = start.x;
		y = start.y;
	}

	if ((edges[y][0] == -1) || (edges[y][0] > x))
		edges[y][0] = x;

	if (edges[y][1] < x)
		edges[y][1] = x;
	return;
}

static void	drawPixel(_positionPair start, uint8_t color, bool b){
	if (b)
		CURRENT_TTY->mode->putPixel((_positionPair){start.y, start.x}, color);
	else
		CURRENT_TTY->mode->putPixel(start, color);
}

void	(*g_pixelsOperation)(_positionPair, uint8_t, bool) = drawPixel;

// takes macros DRAW_PIXEL and STORE_PIXEL as arg
void	switchPixelsMode(bool b){
	if (!b) {
		g_pixelsOperation = drawPixel;
		return;
	}
	g_pixelsOperation = storePixel;
	for (size_t i = 0; i < CURRENT_TTY->mode->screenHeight; i++)
		edges[i][0] = edges[i][1] = -1;
}

// reset = 1 return to drawPixel mode else stay at storePixel mode
// it unsets the stored data
void	drawStoredPixels(uint8_t color, bool reset){
	g_pixelsOperation = drawPixel;
	for (size_t i = 0; i < CURRENT_TTY->mode->screenHeight; i++){
		if (edges[i][0] > -1) // if edges[i][0] > -1 => edges[i][1] > -1
			drawHorizontalLine(i, edges[i][0], edges[i][1], color);
		edges[i][0] = edges[i][1] = -1;
	}
	if (!reset)
		g_pixelsOperation = storePixel;
}