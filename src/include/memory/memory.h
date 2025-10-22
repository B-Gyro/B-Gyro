#pragma once
# include "klibc/types.h"
# include "multiboot.h"

// 0x00000000 -------------------- 0xBFFFFFFF   (Lower 3 GB)
//             User space

// 0xC0000000 -------------------- 0xFFFFFFFF   (Upper 1 GB)
//             Kernel space





# define KERNEL_START			0xC0000000
# define MOV_TO_HIGHER_HALF(x)	(char *)((char *)x + KERNEL_START)

typedef struct physicalMemory {
	uintptr_t	start;
	uintptr_t	end;
	uint32_t	size;
	uint32_t	maxPages;
} _physMemory;

extern _physMemory	g_physMemory;
extern char __KERNEL_VIRTUAL_END[];
extern char __KERNEL_PHYSICAL_END[];

void	initPhysicalMemory(_multibootInfo *info);