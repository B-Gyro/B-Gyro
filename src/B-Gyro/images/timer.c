#include "terminal/vga.h"
#include "time/pit.h"
#include "time/rtc.h"
#include "images/image.h"
#include "drivers/keyboard.h"

# define SEGMENT_SIZE	70
# define SEGMENT_WIDTH	10    // width / 2

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

static uint8_t numbers[10] = {0b1111101, 0b1010000, 0b0110111, 0b1010111, 0b1011010, 0b1001111, 0b1101111, 0b1010001, 0b1111111, 0b1011111};

void	drawNumber(_positionPair pos, char c){

	horizontalSegment(pos, VGA_WHITE * !!(c & 1));
	horizontalSegment((_positionPair){pos.x, pos.y + SEGMENT_SIZE + 2}, VGA_WHITE * !!(c & (1 << 1))); // mid
	horizontalSegment((_positionPair){pos.x, pos.y + ((SEGMENT_SIZE + 2) << 1)}, VGA_WHITE * !!(c & (1 << 2)));

	verticalSegment(pos, VGA_WHITE * !!(c & (1 << 3)));
	verticalSegment((_positionPair){pos.x + SEGMENT_SIZE + 2, pos.y}, VGA_WHITE * !!(c & (1 << 4)));
	verticalSegment((_positionPair){pos.x, pos.y + SEGMENT_SIZE + 2}, VGA_WHITE * !!(c & (1 << 5)));
	verticalSegment((_positionPair){pos.x + SEGMENT_SIZE + 2, pos.y + SEGMENT_SIZE + 2}, VGA_WHITE * !!(c & (1 << 6)));
}

void	drawTimer(void){
	uint16_t	y = 157;
	uint16_t	xHours[2], xMinutes[2], xSeconds[2];

	CURRENT_TTY->mode->clearScreen(1);

	xHours[0] = 5;
	xHours[1] = xHours[0] + SEGMENT_SIZE + SEGMENT_WIDTH * 3;
	xMinutes[0] = xHours[1] + SEGMENT_SIZE + SEGMENT_WIDTH * 3;

	drawFilledRectangle((_positionPair){xMinutes[0], SEGMENT_SIZE - SEGMENT_WIDTH + y}, 10, 10, VGA_WHITE);
	drawFilledRectangle((_positionPair){xMinutes[0], SEGMENT_SIZE + SEGMENT_WIDTH * 2 + y}, 10, 10, VGA_WHITE);

	xMinutes[0] += 20;
	xMinutes[1] = xMinutes[0] + SEGMENT_SIZE + SEGMENT_WIDTH * 3;
	xSeconds[0] = xMinutes[1] + SEGMENT_SIZE + SEGMENT_WIDTH * 3;

	drawFilledRectangle((_positionPair){xSeconds[0], SEGMENT_SIZE - SEGMENT_WIDTH + y}, 10, 10, VGA_WHITE);
	drawFilledRectangle((_positionPair){xSeconds[0], SEGMENT_SIZE + SEGMENT_WIDTH * 2 + y}, 10, 10, VGA_WHITE);

	xSeconds[0] += 20;
	xSeconds[1] = xSeconds[0] + SEGMENT_SIZE + SEGMENT_WIDTH * 3;

	for (size_t hours = 0; hours < 100; hours++){
		drawNumber((_positionPair){xHours[0], y}, numbers[hours / 10]);
		drawNumber((_positionPair){xHours[1], y}, numbers[hours % 10]);
		for (size_t minutes = 0; minutes < 60; minutes++){
			drawNumber((_positionPair){xMinutes[0], y}, numbers[minutes / 10]);
			drawNumber((_positionPair){xMinutes[1], y}, numbers[minutes % 10]);
			for (size_t seconds = 0; seconds < 60; seconds++){
				drawNumber((_positionPair){xSeconds[0], y}, numbers[seconds / 10]);
				drawNumber((_positionPair){xSeconds[1], y}, numbers[seconds % 10]);
				// to do: make it stop on ctrl+c [sleep also]
				if (BIT_IS_SET(g_keyboardData.kbdFlags, KBD_FLAG_CTRL)){
					return;
				}
				sleep(1);
			}
		}
	}	
}
