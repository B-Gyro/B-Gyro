#include "images/image.h"
#include "drivers/vga.h"
#include "klibc/memory.h"
#include "terminal/vga.h"

# define DELTA_X(n0, n1) (n1.x - n0.x)
# define DELTA_Y(n0, n1) (n1.y - n0.y)
# define ABS(n) (n * (n >= 0) + -n * (n < 0))

static void	swap(size_t *a, size_t *b) {
	size_t	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	bresenhamLineDrawing(_positionPair start, _positionPair end, uint8_t color, bool b){
	int32_t	dx, dy;
	int32_t	y;
	int32_t	P;

	y = 1;
	dx = DELTA_X(start, end);
	dy = DELTA_Y(start, end);

	if (dy < 0) {
		dy = -dy;
		y = -1;
	}	

	P = 2 * dy - dx;
	while (start.x <= end.x){
		g_pixelsOperation(start, color, b); // either draw pixel or store data
		start.x++;
		if (P < 0)
			P += 2 * dy;
		else {
			P += 2 * (dy - dx); 
			start.y += y;
		}
	}
}

void	drawLine(_positionPair start, _positionPair end, uint8_t color){
	bool	b;
	int32_t	dx, dy;
	
	b = 0;
	dx = DELTA_X(start, end);
	dy = DELTA_Y(start, end);

	if (ABS(dx) < ABS(dy)) {
		swap(&(start.x), &(start.y));
		swap(&(end.x), &(end.y));
		dx = DELTA_X(start, end);
		b = 1;
	}

	if (dx < 0) {
		swap(&(start.x), &(end.x));
		swap(&(start.y), &(end.y));
	}

	bresenhamLineDrawing(start, end, color, b);
}

// just to simplify process

void	drawHorizontalLine(size_t y, size_t x0, size_t x1, uint8_t color){
	if (x1 < x0)
		swap(&x0, &x1);
	for (size_t i = x0; i <= x1; i++)
		CURRENT_TTY->mode->putPixel((_positionPair){i, y}, color);
}

void	drawVerticalLine(size_t x, size_t y0, size_t y1, uint8_t color){
	if (y1 < y0)
		swap(&y0, &y1);
	for (size_t i = y0; i <= y1; i++)
		CURRENT_TTY->mode->putPixel((_positionPair){x, i}, color);
}