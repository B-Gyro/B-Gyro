#include "images/image.h"
#include "drivers/vga.h"
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

static void	drawPixel(_positionPair start, uint8_t color, bool b){
	if (b)
		CURRENT_TTY->mode->putPixel((_positionPair){start.y, start.x}, color);
	else
		CURRENT_TTY->mode->putPixel(start, color);
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
	while (start.x < end.x){
		drawPixel(start, color, b);
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


void	drawTest(void){
	drawLine((_positionPair){100,100}, (_positionPair){100, 200}, 0x02);
	drawLine((_positionPair){100,100}, (_positionPair){171, 171}, 0x02);
	drawLine((_positionPair){100,100}, (_positionPair){200, 100}, 0x02);
	drawLine((_positionPair){100,100}, (_positionPair){171, 29}, 0x02);
	drawLine((_positionPair){100,100}, (_positionPair){100, 0}, 0x02);
	drawLine((_positionPair){100,100}, (_positionPair){29, 29}, 0x02);
	drawLine((_positionPair){100,100}, (_positionPair){0, 100}, 0x02);
	drawLine((_positionPair){100,100}, (_positionPair){29, 171}, 0x02);
}