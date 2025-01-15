# include "arch/i386/pit.h"

uint32_t	g_ticks = 0;

void	timerHandler(_registers r){
	(void)r;
	g_ticks++;
	if (g_ticks % 100 == 0)
		SERIAL_DEBUG("Tick: %d seconds", g_ticks / 100);
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
