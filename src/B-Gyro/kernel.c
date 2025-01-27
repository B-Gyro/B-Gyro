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

// First method: Using CPUID
int detect_fpu_cpuid() {
    uint32_t eax, ebx, ecx, edx;
    
    // Check if CPUID is supported
    __asm__ volatile(
        "pushfl\n\t"
        "pop %%eax\n\t"
        "mov %%eax, %%ecx\n\t"
        "xor $0x200000, %%eax\n\t"
        "push %%eax\n\t"
        "popfl\n\t"
        "pushfl\n\t"
        "pop %%eax\n\t"
        "push %%ecx\n\t"
        "popfl\n\t"
        : "=a"(eax), "=c"(ecx)
    );
    
    if (eax == ecx) {
        return 0; // CPUID not supported
    }
    
    // Get CPU features
    __asm__ volatile("cpuid" 
        : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) 
        : "a"(1));
    
    return (edx & 0x01); // Bit 0 of EDX indicates FPU presence
}

void init_fpu() {
    // Get current CR0
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    
    // Set CR0.MP (Monitor co-processor, bit 1)
    // Clear CR0.EM (Emulation, bit 2)
    // Clear CR0.TS (Task switched, bit 3)
    cr0 |= 0x2;       // Set MP
    cr0 &= ~0x4;      // Clear EM
    cr0 &= ~0x8;      // Clear TS
    
    // Write back CR0
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
    
    // Initialize FPU
    asm volatile("fninit");
    
    // Optional: Set FPU control word for your specific needs
    uint16_t control_word = 0x037F;  // Default value
    // control_word &= ~0x300;       // Set precision to extended (64-bit)
    // control_word |= 0x3F;         // Mask all FPU exceptions
    asm volatile("fldcw %0" :: "m"(control_word));
}

extern _image *arrayCursors[];
int kmain(void){
	kernelInits();

	 changeVGAMode640x480x16();
	// changeVGAMode13h();
	// changeVGAModeT80x50();
	// changeVGAModeT80x25();

	SERIAL_INFO("FPU is %s", detect_fpu_cpuid() ? "is present" : "not found");
	init_fpu();

	float a,b,c;
	a = 3.4;
	b = 3.5;
	c = a + b;
	(void)c;
	loginScreen(0);

	// SERIAL_PRINT("start");
	// sleep(60);
	// SERIAL_PRINT("done");
	// drawCharacters();
	// drawCursor(&img_defaultCursor, 4, 80);
	sshellStart();

	return 0;
}
