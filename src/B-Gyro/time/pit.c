# include "time/pit.h"

uint32_t	g_ticks = 0;
uint32_t	g_timer = 0;
_time		g_screenTime = {0, 0, 0};
_time		g_currentTime = {0, 0, 0};

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
