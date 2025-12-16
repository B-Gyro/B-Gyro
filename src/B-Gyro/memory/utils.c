#include "memory/memory.h"
#include "memory/paging.h"
#include "klibc/memory.h"
#include "klibc/print.h"


void invalidate(uint32_t addr) {
	asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

uint32_t* getCurrentPageDir(void) {
	uint32_t pageDir;
	asm volatile("mov %%cr3, %0" : "=r"(pageDir));

	return (uint32_t*)(pageDir + KERNEL_START);
}

void setPageDir(uint32_t* pageDir) {
	uint32_t physPageDir = (uint32_t)pageDir - KERNEL_START;
	asm volatile("mov %0, %%eax \n mov %%eax, %%cr3 \n" :: "m"(physPageDir));
	// asm volatile("mov %0, %%cr3" :: "r"(physPageDir));
}

void	printPagesBitMap(int n) {
	for (int i = 0; i < n; i++) {
		for (short bit = 32 - 1; bit >= 0; bit--)
			FILL_BUFFER("%d", !!(pagesBitmap[i] & (1 << bit)));
		
		FILL_BUFFER(" | ");
	}
	FILL_BUFFER("\n");
	PRINT_BUFFER();
}

void	printFramesBitMap(int n) {
	for (int i = 0; i < n; i++) {
		for (short bit = 8 - 1; bit >= 0; bit--)
			FILL_BUFFER("%d", !!(framesBitmap[i] & (1 << bit)));
		
		FILL_BUFFER(" | ");
	}
	FILL_BUFFER("\n");
	PRINT_BUFFER();
}