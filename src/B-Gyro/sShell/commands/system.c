#include "klibc/print.h"
#include "klibc/memory.h"
#include "terminal/tty.h"
#include "sshell/sshell.h"
#include "klibc/strings.h"
#include "arch/i386/ports/portsIO.h"

// https://wiki.osdev.org/%228042%22_PS/2_Controller
// https://www.suwa-koubou.jp/micom/KeyEmuWithGamePad/PS2%20Keyboard.pdf
// https://www.infineon.com/dgdl/Infineon-PS2D_001-13681-Software+Module+Datasheets-v01_02-EN.pdf?fileId=8ac78c8c7d0d8da4017d0fab8b401c89
void reboot(char *args)
{
	char *arg;
	bool isBusy;

	arg = strtok(args, " ");
	while (arg)
	{
		if (!strncmp(arg, "-h", 2) || !strncmp(arg, "--help", 6))
			VGA_PRINT("Usage: ....\n");
		else
			VGA_PRINT("Unknown flag `%s`\n", arg);
		return;
	}

	// Wait until the keyboard controller is not busy
	do
	{
		isBusy = portByteIn(0x64) & 0x02;
	} while (isBusy);

	portByteOut(0x64, 0xFE);

	__asm__ volatile("HLT");
}
