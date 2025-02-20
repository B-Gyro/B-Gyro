# include "time/pit.h"
# include "time/rtc.h"

uint32_t	g_ticks = 0;
uint32_t	g_timer = 0;
_time		g_screenTime = {0, 0, 0, 0};
_time		g_currentTime = {0, 0, 0, 0};

extern bool g_shellMode;

void	updateTime(bool b){
	_time	time;
	
	if (!g_shellMode || MAX_COLUMNS < 73)
		return ;

	getTime(&time);
	if (b || (time.minutes != g_currentTime.minutes)){
		putCharPos(time.minutes % 10 + 48, MAX_COLUMNS - 1, MAX_ROWS);
		putCharPos(time.minutes / 10 + 48, MAX_COLUMNS - 2, MAX_ROWS);
		g_currentTime.minutes = time.minutes;
	}
	putCharPos(':', MAX_COLUMNS - 3, MAX_ROWS);
	if(b || (time.hours != g_currentTime.hours)){
		putCharPos(time.hours % 10 + 48, MAX_COLUMNS - 4, MAX_ROWS);
		putCharPos(time.hours / 10 + 48, MAX_COLUMNS - 5, MAX_ROWS);
		g_currentTime.hours = time.hours;
	}
}

void	timerHandler(_registers r){
	(void)r;
	g_ticks++;
	g_timer++;

	if (!(g_ticks % 100)) {
    	g_screenTime.seconds++;
		if (g_screenTime.seconds > 59) {
			g_screenTime.seconds = 0;
			g_screenTime.minutes++;
			if (g_screenTime.minutes > 59) {
				g_screenTime.minutes = 0;
				g_screenTime.hours++;
			}
			SERIAL_DEBUG("Tick: %d minutes", g_screenTime.minutes);
		}
		if (!(g_screenTime.seconds % 10))
			updateTime(0);
	}
}

void	startTimer(void) {
	setIRQHandler(TIMER_IRQ, timerHandler);

    uint32_t divisor = PIT_FREQUENCY / SLEEP_FREQUENCY;

	setIRQHandler(TIMER_IRQ, timerHandler);
    portByteOut(PIT_CONTROL_PORT, 0x36);
    portByteOut(PIT_CHANNEL0_PORT, divisor & 0xFF);
    portByteOut(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF);
}

void	sleep(uint32_t seconds){
	uint32_t	timer_ticks = g_ticks + (seconds * 100);

	while(g_ticks < timer_ticks) ;
}

void	msleep(uint32_t miliseconds){
	uint32_t	timer_ticks = g_ticks + miliseconds;

	while(g_ticks < timer_ticks) ;
}


static bool stop = 0;
static uint8_t numbers[10] = {0b1111101, 0b1010000, 0b0110111, 0b1010111, 0b1011010, 0b1001111, 0b1101111, 0b1010001, 0b1111111, 0b1011111};

extern _vgaMode g_G640x480x16;
extern void keyboardInterruptHandler(_registers r);
extern void	drawNumber(_positionPair pos, char c);

static void	keyboardHandler(_registers r){
	bool	ctrl;
	uint8_t key;

	(void)r;

	key = portByteIn(KEYBOARD_DATA_PORT);
	if (key == 0x1D)		// CTRL PRESSED
		ctrl = 1;
	else if (key == 0x9D)	// CTRL UNPRESS
		ctrl = 0;
	if (key == 1 || (ctrl && (key == 0x2E)))	// ESC || CTRL+C
		stop = 1;
}

void	drawTimer(void){
	_vgaMode	*tmp = NULL;
	uint16_t	y = 157;
	uint16_t	xHours[2], xMinutes[2], xSeconds[2];
	uint16_t	hours, minutes, seconds;
	
	if (CURRENT_TTY->mode != &g_G640x480x16) {
		tmp = CURRENT_TTY->mode;
		changeVGAMode640x480x16();
	}
	setIRQHandler(KEYBOARD_IRQ, keyboardHandler);

	CURRENT_TTY->mode->clearScreen(1);

	xHours[0] = 5;
	xHours[1] = xHours[0] + SEGMENT_SIZE + SEGMENT_WIDTH * 3;
	xMinutes[0] = xHours[1] + SEGMENT_SIZE + SEGMENT_WIDTH * 3;

	drawFilledSquare((_positionPair){xMinutes[0], SEGMENT_SIZE - SEGMENT_WIDTH + y}, 10, VGA_WHITE);
	drawFilledSquare((_positionPair){xMinutes[0], SEGMENT_SIZE + SEGMENT_WIDTH * 2 + y}, 10, VGA_WHITE);

	xMinutes[0] += 20;
	xMinutes[1] = xMinutes[0] + SEGMENT_SIZE + SEGMENT_WIDTH * 3;
	xSeconds[0] = xMinutes[1] + SEGMENT_SIZE + SEGMENT_WIDTH * 3;

	drawFilledSquare((_positionPair){xSeconds[0], SEGMENT_SIZE - SEGMENT_WIDTH + y}, 10, VGA_WHITE);
	drawFilledSquare((_positionPair){xSeconds[0], SEGMENT_SIZE + SEGMENT_WIDTH * 2 + y}, 10, VGA_WHITE);

	xSeconds[0] += 20;
	xSeconds[1] = xSeconds[0] + SEGMENT_SIZE + SEGMENT_WIDTH * 3;

	drawNumber((_positionPair){xSeconds[0], y}, numbers[0]);
	drawNumber((_positionPair){xSeconds[1], y}, numbers[0]);
	
	g_timer = 0;	
	while (1){
		for (hours = 0; hours < 100 && !stop; hours++){
			drawNumber((_positionPair){xHours[0], y}, numbers[hours / 10]);
			drawNumber((_positionPair){xHours[1], y}, numbers[hours % 10]);
			for (minutes = 0; minutes < 60 && !stop; minutes++){
				drawNumber((_positionPair){xMinutes[0], y}, numbers[minutes / 10]);
				drawNumber((_positionPair){xMinutes[1], y}, numbers[minutes % 10]);
				for (seconds = 0; seconds < 60 && !stop;){
					if (!(g_timer % 100)){
						seconds++;
						drawNumber((_positionPair){xSeconds[0], y}, numbers[(seconds % 60) / 10]);
						drawNumber((_positionPair){xSeconds[1], y}, numbers[(seconds % 60) % 10]);
					}
				}
			}
		}
		if (stop)
			break;
	}
	stop = 0;
	if (tmp)
		tmp->func();
	setIRQHandler(KEYBOARD_IRQ, keyboardInterruptHandler);
}
