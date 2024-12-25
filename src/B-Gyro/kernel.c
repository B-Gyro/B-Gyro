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

#define MAX_USERS 3
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
	{" ", " "}, // 3agzan user
};

void bGyroSetStat(e_bGyroStatus bGStatus)
{
	g_bGyroStats.status = bGStatus;
	updateStatusBar();
}

char *bGyroStatusToString(e_bGyroStatus status)
{
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

void	loginScreen(){
	char user[50], pass[50];
	uint8_t	isValid;

	VGA_PRINT("Welcome ????, who are you !!\r\n");
	prompt("USER:", user);
	keyboardSetKeyPressHandler(passwordKeyHandler);
	prompt("PASSWORD:", pass);
	keyboardResetKeyPressHandler();
	isValid = checkUser(user, pass);
	if (isValid == 0){
		VGA_PRINT("Welcome %s\r\n", user);
		return ;
	}
	VGA_PRINT("Incorrect Info !!!!, retry ...\r\n");
	loginScreen();
}

// always call initTTY(0); before starting to work with terminal
int kmain(void){

	kernelInits();
	loginScreen();
	sshellStart();
	return 0;
}
