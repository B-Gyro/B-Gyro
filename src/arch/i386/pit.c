# include "arch/i386/pit.h"

uint32_t	g_ticks = 0;

uint32_t	g_seconds = 0;
uint32_t	g_minutes = 0;
uint32_t	g_hours = 0;

void updateStatusBar(void);

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
      }
    }
		SERIAL_DEBUG("Tick: %d seconds", g_seconds);
    updateStatusBar();
  }
}

void	startTimer(void) {
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
