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

#define MAX_USERS 4
#define MAX_NAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20

_bGyroStats g_bGyroStats = {
	.OSVersion = "0.1.7",
	.status = B_GYRO_STABLE,
	.isPaginated = 0,
	.mainEBP = 0
};

typedef struct {
    char username[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} User;

User allowedUsers[MAX_USERS] = {
    {"orayn", "hello42"},
    {"faith", "hello42"},
	{"sben-chi", "taha_l_3iyan"},
	{" ", " "}, // 3agzan user
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

void timerHandler(_registers r){
	static uint32_t tick;

	(void)r;
	tick++;
	if (tick % 100 == 0)
		SERIAL_INFO("Tick: %d", tick);
}

void initIRQHandlers(){
	SERIAL_INFO("Initializing IRQ Handlers");
	setIRQHandler(TIMER_IRQ, timerHandler);
	SERIAL_SUCC("IRQ Handlers Initialized");
}

void kernelInits(void){
	testGDT();
	initDescriptorTables();
	testGDT();
	SERIAL_SUCC("Descriptor Tables Initialized");
	initIRQHandlers();
	CURRENT_TTY->index = 0;
	initTerminal();
	SERIAL_SUCC("Kernel Initialized");
	keyboardInit();
	SERIAL_SUCC("Keyboard Initialized");
}

uint8_t	checkUser(char *user, char *pass){
	for (uint8_t i = 0; i < MAX_USERS; i++){
		if (!strncmp(user, allowedUsers[i].username, strlen(user))\
			&& !strncmp(pass, allowedUsers[i].password, strlen(pass)))
			return 0;
	}
	return 1;
}


void	updateCursorLoc(size_t x, size_t y){
	CURRENT_TTY->cursorX = x;
	CURRENT_TTY->posX = x;
	CURRENT_TTY->cursorY = y;
	CURRENT_TTY->posY = y;
	setCursor();
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
	if (isValid == 0){
		VGA_PRINT("Welcome %s\r\n", user);
		return ;
	}
	loginScreen(1);
}

// always call initTTY(0); before starting to work with terminal
int kmain(void){

	kernelInits();
	loginScreen(0);
	sshellStart();
	return 0;
}
