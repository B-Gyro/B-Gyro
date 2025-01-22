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
#include "arch/i386/pit.h"
# include "images/image.h"

_bGyroStats g_bGyroStats = {
	.OSVersion = "0.1.7",
	.status = B_GYRO_STABLE,
	.isPaginated = 0,
	.mainEBP = 0,
	.hasSerialWorking = 0
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

void kernelInits(void){
	initSerial();
	testGDT();
	initDescriptorTables();
	testGDT();
	SERIAL_SUCC("Descriptor Tables Initialized");
	CURRENT_TTY->index = 0;
	initTerminal();
	SERIAL_SUCC("Kernel Initialized");
	keyboardInit();
	SERIAL_SUCC("Keyboard Initialized");
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

void	clearScreen(_vgaMode *vgaMode);

// void	drawCharacters(void);
void	drawCursor(_image *image, size_t x, size_t y);
void	drawCharsImages(_font *image, size_t x, size_t y);



int kmain(void){
	kernelInits();
	startTimer();

	changeVGAMode640x480x16();
	// changeVGAMode13h();
	// changeVGAModeT80x50();
	// clearScreen(CURRENT_TTY->mode);
	// SERIAL_DEBUG("%d:%d\n", MAX_ROWS, MAX_COLUMNS);
	// SERIAL_DEBUG("%d:%d\n", TTY_HEIGHT, TTY_WIDTH);
	// SERIAL_DEBUG("%d:%d\n", FONT_HEIGHT, FONT_WIDTH);

	//loginScreen(0);
	// SERIAL_PRINT("start");
	// sleep(60);
	// SERIAL_PRINT("done");
	// drawCharacters();
	// drawCursor(&defaultCursorImage, 4, 80);
	// drawCharsImages(&g_logo, 4, 4);
	sshellStart();
	return 0;
}
