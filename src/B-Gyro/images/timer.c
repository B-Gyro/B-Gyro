#include "terminal/vga.h"
#include "time/pit.h"
#include "time/rtc.h"
#include "images/image.h"
#include "drivers/keyboard.h"


extern _keyboardData g_keyboardData;

void	horizontalSegment(_positionPair pos, uint8_t color){
	int16_t	x, y;
	
	x = pos.x + SEGMENT_WIDTH; // space between hor. segment and ver. segment
	y = pos.y + SEGMENT_WIDTH - 1;
	for (size_t i = 0; i < SEGMENT_WIDTH; i++){
		drawHorizontalLine(y + i, x + i, x - i + SEGMENT_SIZE, color);
		if (i)
			drawHorizontalLine(y - i, x + i, x - i + SEGMENT_SIZE, color);
	}
	
}

void	verticalSegment(_positionPair pos, uint8_t color){
	int16_t	x, y;
	
	x = pos.x + SEGMENT_WIDTH - 1;
	y = pos.y + SEGMENT_WIDTH; // space between hor. segment and ver. segment
	for (size_t i = 0; i < SEGMENT_WIDTH; i++){
		drawVerticalLine(x + i, y + i, y - i + SEGMENT_SIZE, color);
		if (i)
			drawVerticalLine(x - i, y + i, y - i + SEGMENT_SIZE, color);
	}
}

void	drawNumber(_positionPair pos, char c){

	horizontalSegment(pos, VGA_WHITE * !!(c & 1));
	horizontalSegment((_positionPair){pos.x, pos.y + SEGMENT_SIZE + 2}, VGA_WHITE * !!(c & (1 << 1))); // mid
	horizontalSegment((_positionPair){pos.x, pos.y + ((SEGMENT_SIZE + 2) << 1)}, VGA_WHITE * !!(c & (1 << 2)));

	verticalSegment(pos, VGA_WHITE * !!(c & (1 << 3)));
	verticalSegment((_positionPair){pos.x + SEGMENT_SIZE + 2, pos.y}, VGA_WHITE * !!(c & (1 << 4)));
	verticalSegment((_positionPair){pos.x, pos.y + SEGMENT_SIZE + 2}, VGA_WHITE * !!(c & (1 << 5)));
	verticalSegment((_positionPair){pos.x + SEGMENT_SIZE + 2, pos.y + SEGMENT_SIZE + 2}, VGA_WHITE * !!(c & (1 << 6)));
}
