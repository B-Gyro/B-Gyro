#pragma once

# include "klibc/print.h"

# define PANIC(error)     do {\
	SERIAL_ERR("\033[31m <<< PANIC >>> \033[39m%s", error); \
	bGyroSetStat(B_GYRO_ERROR);\
	__asm__ __volatile__("CLI");\
	while (1) \
		__asm__ __volatile__("HLT");\
} while (0)
