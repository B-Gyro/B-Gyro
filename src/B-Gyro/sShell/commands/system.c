#include "klibc/print.h"
#include "klibc/memory.h"
#include "terminal/tty.h"
#include "sshell/sshell.h"
#include "klibc/strings.h"
#include "arch/i386/ports/portsIO.h"

// https://wiki.osdev.org/%228042%22_PS/2_Controller
// https://www.suwa-koubou.jp/micom/KeyEmuWithGamePad/PS2%20Keyboard.pdf
// https://www.infineon.com/dgdl/Infineon-PS2D_001-13681-Software+Module+Datasheets-v01_02-EN.pdf?fileId=8ac78c8c7d0d8da4017d0fab8b401c89
void reboot(char *args){
	char *arg;
	bool isBusy;

	arg = strtok(args, " ");
	while (arg) {
		if (!strncmp(arg, "-h", 2) || !strncmp(arg, "--help", 6))
			VGA_PRINT("Usage: ....\n");
		else
			VGA_PRINT("Unknown flag `%s`\n", arg);
		return;
	}

	// Wait until the keyboard controller is not busy
	do {
		isBusy = portByteIn(0x64) & 0x02;
	} while (isBusy);

	portByteOut(0x64, 0xFE);

	__asm__ volatile("HLT");
}
void	loginScreen();

void	logout(char *args){

	if (args && *args) {
		VGA_PRINT("%sInvalid options `%s`\n%s", COLOR_RED, args, COLOR_RESET);
		return;
	}

	for (size_t i = 0; i < MAX_TTYS; i++){
		CURRENT_TTY = g_terminal.ttys + i;
		if (CURRENT_TTY->index == i){
			// clear buffer
			CURRENT_TTY->posX = 0;
			CURRENT_TTY->posY = 0;
			clearTTY(SCREEN_SIZE);

			// clear history
			CURRENT_TTY->history->size = 0;
			CURRENT_TTY->history->last = CURRENT_TTY->history->first;
			CURRENT_TTY->history->current = CURRENT_TTY->history->first;
			bigBzero(CURRENT_TTY->history->last->ptr, MAX_KEYBOARD_BUFFER);

			keyboardClearBuffer();
		}
	}

	CURRENT_TTY = g_terminal.ttys;
	
	loginScreen();
}

int32_t	getUserID(char *username){

	for (uint32_t i = 0; i < g_terminal.usersNbr; i++){
		if (!strcmp(username, g_users[i].username))
			return (i);
	}
	return (-1);
}

void	adduser(char *args){
	char	*arg, pass[MAX_KEYBOARD_BUFFER];
	size_t	nameSize, passSize;

	if (!args || !*args) {
		printError("Username must be provided to execute the command.");
		return;
	}

	if (g_terminal.usersNbr >= MAX_USERS){
		printError("Maximum user limit reached.");
		return;
	}

	arg = strtok(args, " ");
	nameSize = strlen(arg);
	if (nameSize > MAX_NAME_LENGTH){
		printError("Name exceeds the maximum allowed size.");
		return ;
	}

	if (getUserID(arg) > -1){
		printError("Username already exists.");
		return ;
	}

	keyboardSetKeyPressHandler(passwordKeyHandler);
	while (1337){
		bzero(pass, MAX_KEYBOARD_BUFFER);
		prompt("PASSWORD:", pass);

		passSize = strlen(pass);
		if (passSize > MAX_PASSWORD_LENGTH){
			printError("Password exceeds the maximum allowed size.");
			continue; ;
		}

		if (!passSize){
			printError("Password can't be empty.");
			continue; ;
		}

		break;
	}
	keyboardResetKeyPressHandler();
	


	strlcpy(g_users[g_terminal.usersNbr].username, arg, nameSize);
	strlcpy(g_users[g_terminal.usersNbr].password, pass, passSize);
	g_users[g_terminal.usersNbr].id = g_terminal.usersNbr;
	g_terminal.usersNbr++;
}