#include "memory/memory.h"
#include "memory/paging.h"
#include "klibc/memory.h"
#include "klibc/print.h"

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