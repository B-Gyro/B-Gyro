#include "images/image.h"
#include "drivers/vga.h"
// #include "terminal/vga.h"
#include "klibc/print.h"

// FILLED *********************************************************
void	drawFilledTriangle(_positionPair p1, _positionPair p2, _positionPair p3, uint8_t color){
	switchPixelsMode(STORE_PIXEL);
	drawLine(p1, p2, color);
	drawLine(p1, p3, color);
	drawLine(p2, p3, color);
	drawStoredPixels(color, 1);
}

void	drawFilledRectangle(_positionPair pos, size_t width, size_t height, uint16_t color){
	size_t	size;
	size_t	x0, x1;

	x0 = pos.x;
	x1 = pos.x + width - 1;
	size = pos.y + height;
	for (size_t y = pos.y; y < size; y++)
		drawHorizontalLine(y, x0, x1, color);
}

void	drawFilledSquare(_positionPair pos, size_t width, uint16_t color){
	drawFilledRectangle(pos, width, width, color);
}

void	drawFilledRhombus(_positionPair pos, size_t diagX, size_t diagY, uint8_t color){
	size_t r1, r2;

	r1 = diagX / 2 - 1;
	r2 = diagY / 2 - 1;
	
	switchPixelsMode(STORE_PIXEL);
	drawLine((_positionPair){pos.x, pos.y + r2}, (_positionPair){pos.x + r1, pos.y}, color);
	drawLine((_positionPair){pos.x, pos.y + r2}, (_positionPair){pos.x - r1, pos.y}, color);
	drawLine((_positionPair){pos.x, pos.y - r2}, (_positionPair){pos.x + r1, pos.y}, color);
	drawLine((_positionPair){pos.x, pos.y - r2}, (_positionPair){pos.x - r1, pos.y}, color);
	drawStoredPixels(color, 1);
}

// EMPTY *********************************************************
void	drawTriangle(_positionPair p1, _positionPair p2, _positionPair p3, uint8_t color){
	drawLine(p1, p2, color);
	drawLine(p1, p3, color);
	drawLine(p2, p3, color);
}

void	drawRectangle(_positionPair pos, size_t width, size_t height, uint16_t color){
	size_t	y, x;

	x = pos.x + width - 1;
	y = pos.y + height - 1;
	drawHorizontalLine(pos.y, pos.x, x, color);
	drawHorizontalLine(y, pos.x, x, color);
	drawVerticalLine(pos.x, pos.y, y, color);
	drawVerticalLine(x, pos.y, y, color);
}

void	drawSquare(_positionPair pos, size_t width, uint16_t color){
	drawRectangle(pos, width, width, color);
}

void	drawRhombus(_positionPair pos, size_t diagX, size_t diagY, uint8_t color){
	size_t r1, r2;

	r1 = diagX / 2 - 1;
	r2 = diagY / 2 - 1;
	
	drawLine((_positionPair){pos.x, pos.y + r2}, (_positionPair){pos.x + r1, pos.y}, color);
	drawLine((_positionPair){pos.x, pos.y + r2}, (_positionPair){pos.x - r1, pos.y}, color);
	drawLine((_positionPair){pos.x, pos.y - r2}, (_positionPair){pos.x + r1, pos.y}, color);
	drawLine((_positionPair){pos.x, pos.y - r2}, (_positionPair){pos.x - r1, pos.y}, color);
}
