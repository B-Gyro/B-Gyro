#pragma once

# include "arch/i386/cpu/descriptorTables.h"
# include "terminal/_data.h"
# include "arch/i386/ports/portsIO.h"
# include "time/rtc.h"
# include "terminal/vga.h"

# define PIT_FREQUENCY		1193180 	// PIT clock frequency (1.19318 MHz)
# define SLEEP_FREQUENCY	100			// Desired sleep frequency in Hz 1 tick == 10 ms

# define PIT_CONTROL_PORT	0x43

# define PIT_CHANNEL0_PORT	0x40
# define PIT_CHANNEL1_PORT	0x41
# define PIT_CHANNEL2_PORT	0x42


// timer draw
# define SEGMENT_SIZE	70
# define SEGMENT_WIDTH	10    // width / 2
// extern uint32_t	g_ticks;

void	startTimer(void);
void	sleep(uint32_t seconds);
void	msleep(uint32_t miliseconds);
void	updateTime(bool b);
void	drawTimer(void);
