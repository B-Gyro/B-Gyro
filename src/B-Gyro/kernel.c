#include "bGyro.h"
#include "images/image.h"
#include "klibc/print.h"
#include "terminal/vga.h"
#include "terminal/tty.h"
#include "sshell/sshell.h"
#include "klibc/strings.h"
#include "arch/i386/pit.h"
#include "terminal/_data.h"
#include "drivers/keyboard.h"
#include "terminal/terminal.h"
#include "arch/i386/cpu/descriptorTables.h"

_bGyroStats g_bGyroStats = {
	.OSVersion = "0.1.7",
	.status = B_GYRO_STABLE,
	.isPaginated = 0,
	.mainEBP = 0,
	.hasSerialWorking = 0,
	.isTTYMode = 1
};

void bGyroSetStat(e_bGyroStatus bGStatus) {
	g_bGyroStats.status = bGStatus;
	updateStatusBar();
}

char *bGyroStatusToString(e_bGyroStatus status) {
	switch (status){
	case B_GYRO_STABLE:
		return COLOR_GREEN "STABLE" COLOR_DEFAULT;
	case B_GYRO_ERROR:
		return COLOR_LIGHT_RED "ERROR" COLOR_DEFAULT;
	default:
		return COLOR_RED "UNKNOWN" COLOR_DEFAULT;
	}
}

void testGDT(){
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

void EnableFPU() {
    // Enable FPU by modifying CR0
    __asm__ volatile (
        "mov %%cr0, %%eax\n\t"  // Move CR0 into EAX
        "and $0xFFFB, %%eax\n\t" // Clear the EM bit (bit 2)
        "or $0x2, %%eax\n\t"    // Set the MP bit (bit 1)
        "mov %%eax, %%cr0\n\t"  // Move EAX back into CR0
        :
        :
        : "eax"
    );
    // Initialize the FPU
    __asm__ volatile ("fninit");
}

void kernelInits(void){
	initSerial();
	testGDT();
	initDescriptorTables();
	testGDT();
	startTimer();
	SERIAL_SUCC("Descriptor Tables Initialized");
	CURRENT_TTY->index = 0;
	initTerminal();
	SERIAL_SUCC("Kernel Initialized");
	keyboardInit();
	SERIAL_SUCC("Keyboard Initialized");
	EnableFPU();
	SERIAL_SUCC("FPU Enabled");
}

void	loginScreen(bool alreadyPrompted){
	char user[50], pass[50];
	uint8_t	isValid;

	
	clearTTY(SCREEN_SIZE);
	drawImage(&img_logo, 4, 100);
	if (alreadyPrompted)
		putStrPos("Incorrect USER or PASSWORD",31, 10);
	putStrPos("------------------------------",30, 11);
	putStrPos("|                            |",30, 12);
	putStrPos("|                            |",30, 13);
	putStrPos("|                            |",30, 14);
	putStrPos("|                            |",30, 15);
	putStrPos("------------------------------", 30, 16);
	updateCursorLoc(32, 13);
	prompt("USER:", user);
	keyboardSetKeyPressHandler(passwordKeyHandler);
	updateCursorLoc(32, 14);
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

void	drawTest(void);

extern _image *arrayCursors[];

void	draw8Pixels640x480x16(_positionPair pos, uint8_t byte, uint8_t forgroundColor, uint8_t backgroundColor);

int kmain(void){
	kernelInits();

	changeVGAMode640x480x16();
	//changeVGAModeT80x50();
	//changeVGAModeT80x25();
	//loginScreen(0);
	//sshellStart();
	//drawSquare()
	for (size_t i = 0; i < 16; i++){
		draw8Pixels640x480x16((_positionPair){.x = 50, .y = 50 + i}, CURRENT_TTY->font->pixels[48][i], 15, 0);
	}
	// drawTest();
	return 0;
}
