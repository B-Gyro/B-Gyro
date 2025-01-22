# include "arch/i386/pit.h"

uint32_t	g_ticks = 0;

uint32_t	g_seconds = 0;
uint32_t	g_minutes = 0;
uint32_t	g_hours = 0;


void	updateTimer(uint8_t nbr, uint8_t x, uint8_t y){
	putCharPos(nbr % 10 + 48, x, y);
	putCharPos(nbr / 10 + 48, x - 1, y);
}

void	printTimer(void){
	updateTimer(g_seconds, MAX_COLUMNS - 1, MAX_ROWS);
	putCharPos(':', MAX_COLUMNS - 3, MAX_ROWS);
	updateTimer(g_minutes, MAX_COLUMNS - 4, MAX_ROWS);
	putCharPos(':', MAX_COLUMNS - 6, MAX_ROWS);
	updateTimer(g_hours, MAX_COLUMNS - 7, MAX_ROWS);
}

void	timerHandler(_registers r){
	(void)r;
	g_ticks++;
  
	if (!(g_ticks % 100)) {
    	g_seconds++;
		if (g_seconds > 59) {
			g_seconds = 0;
			g_minutes++;
			if (g_minutes > 59) {
				g_minutes = 0;
				g_hours++;
				updateTimer(g_hours, MAX_COLUMNS - 7, MAX_ROWS);
			}
			SERIAL_DEBUG("Tick: %d minutes", g_minutes);
			updateTimer(g_minutes, MAX_COLUMNS - 4, MAX_ROWS);
		}
		updateTimer(g_seconds, MAX_COLUMNS - 1, MAX_ROWS);
	}
}

void	startTimer(void) {
	setIRQHandler(TIMER_IRQ, timerHandler);

    uint32_t divisor = PIT_FREQUENCY / SLEEP_FREQUENCY;

    portByteOut(PIT_CONTROL_PORT, 0x36);
    portByteOut(PIT_CHANNEL0_PORT, divisor & 0xFF);
    portByteOut(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF);
}

void	sleep(uint32_t seconds){
	uint32_t	timer_ticks = g_ticks + (seconds * 100);

	while(g_ticks < timer_ticks) ;
}
