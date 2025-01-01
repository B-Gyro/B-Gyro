#include "terminal/_data.h"
#include "terminal/terminal.h"
#include "terminal/tty.h"
#include "terminal/vga.h"

#include "klibc/print.h"
#include "klibc/strings.h"
#include "drivers/keyboard.h"
#include "sshell/sshell.h"
#include "arch/i386/cpu/descriptorTables.h"
#include "bGyro.h"

_bGyroStats g_bGyroStats = {
	.OSVersion = "0.1.7",
	.status = B_GYRO_STABLE,
	.isPaginated = 0,
	.mainEBP = 0
};

void bGyroSetStat(e_bGyroStatus bGStatus) {
	g_bGyroStats.status = bGStatus;
	updateStatusBar();
}

char *bGyroStatusToString(e_bGyroStatus status) {
	switch (status)
	{
	case B_GYRO_STABLE:
		return COLOR_GREEN "STABLE" COLOR_DEFAULT;
	case B_GYRO_ERROR:
		return COLOR_LIGHT_RED "ERROR" COLOR_DEFAULT;
	default:
		return COLOR_RED "UNKNOWN" COLOR_DEFAULT;
	}
}

void testGDT()
{
	uint32_t cr0;
	_gdtPtr gdt = {.base = 0, .limit = 0};

	SERIAL_INFO("Testing GDT");
	asm volatile("sgdt %0" : : "m"(gdt));
	SERIAL_DEBUG("GDT Base Address: %08p", gdt.base);
	SERIAL_DEBUG("GDT Limit: %p", gdt.limit);
	SERIAL_DEBUG("GDT Entries Count: %d", (gdt.limit + 1) / sizeof(_gdtEntry));
	/*Testing if paging Enabled*/
	asm volatile("mov %%cr0, %0" : "=r"(cr0));
	SERIAL_DEBUG("Paging is currently: %s", (cr0 & 0x80000000) ? "ENABLED" : "DISABLED");
	SERIAL_INFO("GDT Test Done");
}

void sleep(uint8_t n){
	uint32_t X = 2500000; // 6 * 10^8 for 1s

	X *= n;
	for (uint32_t x = 0; x < X; x++);
}

/* Appreciate my old self for defining all this for the old Yona project */
/* Appreciate my 2nd old self for borrowing all of this from my old project */

# define C0_DATA 		0x40	// channel	1
# define C1_DATA 		0x41	// ..		2
# define C2_DATA 		0x42	// ..		3
// Mode/Command register (write only, a read is ignored)
# define COMMAND_REG	0x43
/*								CHANNEL (bits 6 and 7)								*/
	# define PIT_CHANNEL0		0B00000000 // select channel 0
	# define PIT_CHANNEL1		0B01000000 // select channel 1
	# define PIT_CHANNEL2		0B10000000 // select channel 2
	# define PIT_READ_BACK		0B11000000 // read-back command (8254 only)
/*								ACCESS (bits 6 and 7)								*/
	# define PIT_LATCH_COUNT	0B00000000 // latch count value command
	# define PIT_LB				0B00010000 // low byte only
	# define PIT_HB				0B00100000 // high byte only
	# define PIT_LHB			0B00110000 // low and high bytes
/*								OPERATING (bits 1 to 3)								*/
	# define MODE0				0B00000000 // interrupt on terminal count
	# define MODE1				0B00000010 // hardware re-triggerable one-shot
	# define MODE2				0B00000100 // rate generator
	# define MODE3				0B00000110 // square wave generator
	# define MODE4				0B00001000 // software triggered strobe
	# define MODE5				0B00001010 // hardware triggered strobe
	# define MODE6				0B00001100 // rate generator (same as mode 2)
	# define MODE7				0B00001110 // square wave generator (same as mode 3)
/*								COUNT (bits 1 to 3)								*/
	# define PIT_BINARY			0B00000000 // 16 bit binary
	# define PIT_BCD			0B00000001 // 4 digit BCD

# define CONFIGURE_PIT(CHANNEL, ACCESS_MODE, OPERATING_MODE, BCD_BINARY) (CHANNEL | ACCESS_MODE | OPERATING_MODE | BCD_BINARY)

typedef struct sysClock
{
	uint8_t		s;
	uint8_t		m;
	uint8_t		h;
	uint8_t		d;
	uint8_t		mo;
	uint32_t	y;
	uint32_t	frequency;
	uint32_t	msElapsedFromBoot;
} _sysClock;

void	initTimer(uint32_t frequency);

_sysClock date = {
	.s = 0,
	.m = 0,
	.h = 0,
	.d = 0,
	.mo = 0,
	.y = 0,
	.frequency = 1000,
	.msElapsedFromBoot = 0
};

void	tick(_registers r){
	(void)r;
	date.msElapsedFromBoot++;
}


void	initTimer(uint32_t frequency){
	uint16_t	divisor;

	divisor = 1193180 / frequency;
	date.frequency = frequency;
	// Configuration of the PIT to send the divisor to CHANNEL 0
	portByteOut(COMMAND_REG, CONFIGURE_PIT(PIT_CHANNEL0, PIT_LHB, MODE3, PIT_BINARY));
	// send the frequency divisor
	portByteOut(C0_DATA, divisor & 0xff);
	portByteOut(C0_DATA, (divisor >> 8) & 0xff);
	setIRQHandler(0, tick);
}

void kernelInits(void){
	//initSerial();
	char	*videoMemory = (char *)VIDEO_ADDRESS;
	videoMemory[0] = '0';
	testGDT();
	videoMemory[2] = '1';
	initDescriptorTables();
	videoMemory[4] = '2';
	testGDT();
	videoMemory[6] = '3';
	SERIAL_SUCC("Descriptor Tables Initialized");
	videoMemory[8] = '4';
	CURRENT_TTY->index = 0;
	initTerminal();
	videoMemory[10] = '5';
	SERIAL_SUCC("Kernel Initialized");
	keyboardInit();
	videoMemory[12] = '6';
	SERIAL_SUCC("Keyboard Initialized");
	videoMemory[14] = '7';
	initTimer(1000);
	videoMemory[16] = '8';
	SERIAL_SUCC("Timer Initialized");
	videoMemory[18] = '9';
}

void	loginScreen(bool alreadyPrompted){
	char user[50], pass[50];
	uint8_t	isValid;

	
	clearTTY(SCREEN_SIZE);
	if (alreadyPrompted)
		putStrPos("Incorrect USER or PASSWORD",26, 7);
	putStrPos("------------------------------",24, 8);
	putStrPos("|                            |",24, 9);
	putStrPos("|                            |",24, 10);
	putStrPos("|                            |",24, 11);
	putStrPos("|                            |",24, 12);
	putStrPos("------------------------------", 24, 13);
	updateCursorLoc(26, 10);
	prompt("USER:", user);
	keyboardSetKeyPressHandler(passwordKeyHandler);
	updateCursorLoc(26, 11);
	prompt("PASSWORD:", pass);
	keyboardResetKeyPressHandler();
	isValid = checkUser(user, pass);
	clearTTY(SCREEN_SIZE);
	if (isValid){
		VGA_PRINT("Welcome %s\r\n", user);
		return ;
	}
	loginScreen(1);
}

// always call initTerminal; before starting to work with terminal
int kmain(void){
	
	kernelInits();
	loginScreen(0);
	sshellStart();
	return 0;
}
