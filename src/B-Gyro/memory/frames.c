#include "memory/paging.h"
#include "memory/memory.h"
#include "klibc/memory.h"
#include "klibc/print.h"

// frame per bit
uint8_t	framesBitmap[THEORICAL_FRAMES_NUMBER / UINT8_SIZE]; 

// next free frame algo | call it with end = 0
uint32_t	allocFrame(uint32_t end){
	static uint32_t	last;

	end = (!end ? REAL_FRAMES_NUMBER / UINT8_SIZE : end);

	for (uint32_t i = last; i < end; i++) {
		if (framesBitmap[i] != MAX_UINT8) {
			for (short bit = UINT8_SIZE - 1; bit >= 0; bit--) {
				if (!(framesBitmap[i] & (1 << bit))) {
					framesBitmap[i] |= (1 << bit);
					g_physMemory.freePages--;
					last = i + !bit;
					// SERIAL_DEBUG("i: %d\n", (i * UINT8_SIZE + (UINT8_SIZE - 1 - bit)));
					return ((uint32_t)((char *)g_physMemory.start + (i * UINT8_SIZE + (UINT8_SIZE - 1 - bit))  * PAGE_SIZE));
				}	
			}
		}
	}

	if (last != 0) {
		end = last;
		last = 0;
		allocFrame(end);
	}

	return NULL;
}

void	freeFrame(uint32_t ptr){
	char		b;
	uint32_t	index;

	index = (uint32_t)(((char *)ptr - (char*)g_physMemory.start) / PAGE_SIZE);
	// SERIAL_DEBUG("i: %d\n", index);
	// SERIAL_DEBUG("i: %d\n", 7 - (index % UINT8_SIZE));

	b = (1 << (7 - (index % UINT8_SIZE)));
	if (framesBitmap[index / UINT8_SIZE] & b) {
		g_physMemory.freePages++;
		framesBitmap[index / UINT8_SIZE] &= ~b;
	}
}

uint32_t	getFrameAddr(uint32_t vAddr) {
	uint32_t pdIndex = PDE_INDEX(vAddr);
	uint32_t ptIndex = PTE_INDEX(vAddr);

	uint32_t* pageDir = (uint32_t*)REC_PAGE_DIR_ADD;
	if (!(pageDir[pdIndex] & FLAG_PAGE_PRESENT))
		return NULL; // No page table

	uint32_t* pageTab = (uint32_t*)PAGEDIR_ENTRY(pdIndex);
    // to do: check whether the PT entry is present.

    return (uint32_t)((pageTab[ptIndex] & ~0xFFF) + ((unsigned long)vAddr & 0xFFF));
}