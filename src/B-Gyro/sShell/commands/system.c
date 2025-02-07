#include "klibc/print.h"
#include "klibc/memory.h"
#include "terminal/tty.h"
#include "sshell/sshell.h"
#include "klibc/strings.h"
#include "arch/i386/ports/portsIO.h"
#include "arch/i386/pci/pci.h"

// https://wiki.osdev.org/%228042%22_PS/2_Controller
// https://www.suwa-koubou.jp/micom/KeyEmuWithGamePad/PS2%20Keyboard.pdf
// https://www.infineon.com/dgdl/Infineon-PS2D_001-13681-Software+Module+Datasheets-v01_02-EN.pdf?fileId=8ac78c8c7d0d8da4017d0fab8b401c89
void reboot(char *args){
	char *arg;
	bool isBusy;

	arg = strtok(args, " ");
	if (arg) {
		if (!strncmp(arg, "-h", 2) || !strncmp(arg, "--help", 6))
			FILL_BUFFER("Usage: ....\n");
		else
			FILL_BUFFER("Unknown flag `%s`\n", arg);
		return;
	}

	// Wait until the keyboard controller is not busy
	do {
		isBusy = portByteIn(0x64) & 0x02;
	} while (isBusy);

	portByteOut(0x64, 0xFE);

	__asm__ volatile("HLT");
}

void	clearData(void){
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
}

void	logout(char *args){

	if (args && *args) {
		FILL_BUFFER("%sInvalid options `%s`\n%s", COLOR_RED, args, COLOR_RESET);
		return;
	}

	clearData();

	g_users.current = NULL;
	loginScreen(0);
}

void	adduser(char *args){
	char	*arg, pass[MAX_KEYBOARD_BUFFER];
	size_t	nameSize, passSize;

	if (!args || !*args) {
		printError("Username must be provided to execute the command.");
		return;
	}

	if (g_users.size >= MAX_USERS){
		printError("Maximum user limit reached.");
		return;
	}

	arg = strtok(args, " ");
	nameSize = strlen(arg);
	if (nameSize > MAX_NAME_LENGTH){
		printError("Name exceeds the maximum allowed size.");
		return ;
	}

	if (getUserID(arg)){
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

	g_users.last = g_users.last->next;
	strlcpy(((_user *)g_users.last->ptr)->username, arg, nameSize);
	strlcpy(((_user *)g_users.last->ptr)->password, pass, passSize);
	((_user *)g_users.last->ptr)->id = g_id++;
	g_users.size++;
}

void	deluser(char *args){
	char	*arg, pass[MAX_KEYBOARD_BUFFER];
	_node	*user;

	if (!args || !*args){
		printError("Username must be provided to execute the command.");
		return;
	}

	arg = strtok(args, " ");
	user = getUserID(arg);
	if (!user){
		printError("User doesn't exist.");
		return;
	}

	if (!((_user *)user->ptr)->id){
		printError("User root can't be deleted.");
		return;
	}

	if (user == g_users.current){
		printError("You cannot delete your own user while logged in.");
		return;
	}

	keyboardSetKeyPressHandler(passwordKeyHandler);
	for (uint8_t i = 0; i < 3; i++){
		bzero(pass, MAX_KEYBOARD_BUFFER);
		prompt("Enter your password:", pass);

		if (strcmp(((_user *)g_users.current->ptr)->password, pass)){
			
			printError("Wrong password.");
			continue;
		}



		if (user != g_users.last){
			// remove user from list
			user->previous->next = user->next;
			user->next->previous = user->previous;

			// add user to the end of the list
			g_users.last->next = user;
			g_users.first->previous = user;
			user->previous = g_users.last;
			user->next = g_users.first;
			g_users.last = user;
		}

		g_users.size--;
		break;
	}
	keyboardResetKeyPressHandler();
}

void	su(char *args){
	char	*arg, pass[MAX_KEYBOARD_BUFFER];
	_node	*user;

	if (!args || !*args){
		printError("Username must be provided to execute the command.");
		return;
	}

	arg = strtok(args, " ");
	user = getUserID(arg);
	if (!user){
		printError("User doesn't exist.");
		return;
	}

	if (user == g_users.current){
		printError("You are already logged in.");
		return;
	}

	keyboardSetKeyPressHandler(passwordKeyHandler);
	for (uint8_t i = 0; i < 3; i++){
		bzero(pass, MAX_KEYBOARD_BUFFER);
		prompt("PASSWORD:", pass);

		if (strcmp(((_user *)user->ptr)->password, pass)){
			
			printError("Wrong password.");
			continue;
		}
		clearData();
		g_users.current = user;
		break;
	}
	keyboardResetKeyPressHandler();
}

void	lspci(char *args){
	if (args && (strcmp(args, "-h") || strcmp(args, "--help"))){
		FILL_BUFFER("Description: Lists all PCI devices\n");
		FILL_BUFFER("Format: [bus:slot:function] vendorID deviceID class subclass\n");
		return;
	}
	getPCIDevices();	
}