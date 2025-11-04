#include "bGyro.h"
#include "system.h"
#include "images/image.h"
#include "klibc/print.h"
#include "terminal/vga.h"
#include "terminal/tty.h"
#include "sshell/sshell.h"
#include "klibc/strings.h"
#include "time/pit.h"
#include "terminal/_data.h"
#include "memory/memory.h"
#include "memory/paging.h"
#include "memory/multiboot.h"
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
	initTerminal();
	SERIAL_SUCC("Terminal Initialized");
	// testGDT();
	initDescriptorTables();
	SERIAL_SUCC("Descriptor Tables Initialized");
	// testGDT();
	startTimer();
	SERIAL_SUCC("Timer Initialized");
	keyboardInit();
	SERIAL_SUCC("Keyboard Initialized");
	// EnableFPU();
	// SERIAL_SUCC("FPU Enabled");
}

char *prompt_(char *promtMessage, char *buffer);

void	loginScreen(bool alreadyPrompted){
	char user[30], pass[30];
	uint8_t	isValid;

	if (!alreadyPrompted){
		clearTTY(FULL_SCREEN_SIZE);
		drawImage(&img_logo, 4, 100);
		drawImage(&img_42, 551, 410);
		drawRectangle((_positionPair){35 * FONT_WIDTH, 11 * FONT_HEIGHT}, 44 * FONT_WIDTH, 6 * FONT_HEIGHT, VGA_BRIGHT_WHITE);
		drawRectangle((_positionPair){35 * FONT_WIDTH + 1, 11 * FONT_HEIGHT + 1}, 44 * FONT_WIDTH - 2, 6 * FONT_HEIGHT - 2, VGA_BRIGHT_WHITE);
		drawRectangle((_positionPair){35 * FONT_WIDTH + 3, 11 * FONT_HEIGHT + 3}, 44 * FONT_WIDTH - 6, 6 * FONT_HEIGHT - 6, VGA_BRIGHT_WHITE);
		drawRectangle((_positionPair){35 * FONT_WIDTH + 4, 11 * FONT_HEIGHT + 4}, 44 * FONT_WIDTH - 8, 6 * FONT_HEIGHT - 8, VGA_BRIGHT_WHITE);
	}
	MAX_KEYBOARD_BUFFER = 30;
	updateCursorLoc(37, 13);
	prompt_("USER:", user);
	keyboardSetKeyPressHandler(passwordKeyHandler);
	updateCursorLoc(37, 14);
	prompt_("PASSWORD:", pass);
	keyboardResetKeyPressHandler();
	isValid = checkUser(user, pass);
	if (isValid){
		clearTTY(SCREEN_SIZE);
		VGA_PRINT("Welcome %s\r\n", user);
		MAX_KEYBOARD_BUFFER = DEFAULT_MAX_KEYBOARD_BUFFER;
		sshellStart();
		return ;
	}
	putStrPos("\033[91mIncorrect USER or PASSWORD\033[39m",41, 10);
	putStrPos("                                        ", 37, 13);
	putStrPos("                                        ", 37, 14);
	loginScreen(1);
}

void	printPagesBitMap(int n);
void	printFramesBitMap(int n);

int kmain(uint32_t magicNbr, _multibootInfo *multibootInfo){
	kernelInits();

	if (magicNbr != BOOTLOADER_MAGIC_NBR) {
		// We weren't booted by a compliant bootloader!
		PANIC("Unvalid magic number.");
		return (0);
	}

	initPhysicalMemory((_multibootInfo *)MOV_TO_HIGHER_HALF(multibootInfo));
	initVirtualMemory();

	char	*add1 = (char *)kmmap(PAGE_SIZE);

	(void)add1;
	add1[10] = 0;
	VGA_PRINT("%p\n", add1);

	printFramesBitMap(5);
	char	*add2 = (char *)kmmap(PAGE_SIZE);

	VGA_PRINT("+++++\n");

	// PRINT_BUFFER();
	// printFramesBitMap(5);
	// kunmap(((uint32_t)add2));
	// printFramesBitMap(5);
	// SERIAL_DEBUG("==>%p", getPhysicalAddr((uint32_t)add1));

	// printFramesBitMap(5);
	// kunmap(((uint32_t)add2));
	// printFramesBitMap(5);
	VGA_PRINT("Done\n");
	VGA_PRINT("ERROR0\n");

	add2[10] = 0;
	VGA_PRINT("ERROR\n");

	VGA_PRINT("%p", add2);
	VGA_PRINT("ERROR\n");

	// SERIAL_INFO("%010p", __KERNEL_VIRTUAL_END);
	
	// changeVGAMode640x480x16();
	// loginScreen(0);
	sshellStart();
	
	return 0;
}