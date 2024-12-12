# pragma once
# include "terminal/terminal.h"
# include "arch/i386/ports/serialCommunication/serialIO.h"

typedef uint8_t (*putCharFnc)(char);

typedef struct buff
{
	char		*buff;
	uint32_t	index;
}_buff;

uint32_t     print(putCharFnc fnc, char *fmtString, ...);

#define SERIAL_PRINT(FMT_STRING, ...) print(serialPutChar, FMT_STRING, ##__VA_ARGS__)

// Prints a string to the VGA doesn't update the state of the yonaData.
#define SERIAL_INFO(FMT_STRING, ...)    print(serialPutChar, "\033[34mINFO \033[39m [%s] " FMT_STRING "\n", __func__, ##__VA_ARGS__)
#define SERIAL_SUCC(FMT_STRING, ...)    print(serialPutChar, "\033[92mSUCC \033[39m [%s] " FMT_STRING "\n", __func__, ##__VA_ARGS__)
#define SERIAL_ERR(FMT_STRING, ...)     print(serialPutChar, "\033[31mERR  \033[39m [%s] " FMT_STRING "\n", __func__, ##__VA_ARGS__)
#define SERIAL_DEBUG(FMT_STRING, ...)   print(serialPutChar, "\033[93mDEBUG\033[39m [%s] " FMT_STRING "\n", __func__, ##__VA_ARGS__)


#define VGA_PRINT(FMT_STRING, ...) print(putChar, FMT_STRING, ##__VA_ARGS__)
