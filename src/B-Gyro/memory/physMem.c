#include "memory/memory.h"
#include "memory/paging.h"
#include "klibc/memory.h"
#include "klibc/print.h"

_physMemory	g_physMemory = {0, 0, 0, 0, 0};

void	initPhysicalMemory(_multibootInfo *info) {
	char *g_memoryTypes[5] = {"\033[92mAVAILABLE\033[0m", "\033[95mRESERVED\033[0m", "\033[95mACPI RECLAIMABLE\033[0m", "\033[91mNVS\033[0m", "\033[91mBAD RAM\033[0m"};

	for(uint32_t i = 0; i < info->mmapLength; i += sizeof(_memoryMap)) {
        _memoryMap* mmmt = (_memoryMap*) (MOV_TO_HIGHER_HALF(info->mmapAddr + i));

        SERIAL_INFO("Addr: 0x%08x | Len: %7d kb | %4d mb | Type: %s",
             mmmt->addr.low, mmmt->len.low / 1024, mmmt->len.low / (1024 * 1024), g_memoryTypes[mmmt->type - 1]);
		
		if (mmmt->len.low > g_physMemory.size) {
			g_physMemory.size = mmmt->len.low;
			g_physMemory.start = mmmt->addr.low;
			g_physMemory.end = g_physMemory.start + g_physMemory.size;
		}
    }

	if (g_physMemory.start < (uintptr_t)__KERNEL_PHYSICAL_END)
		g_physMemory.start = (uintptr_t)__KERNEL_PHYSICAL_END;
	g_physMemory.start = ALIGN_UP(g_physMemory.start);
	g_physMemory.end = ALIGN_DOWN(g_physMemory.end);

	g_physMemory.maxPages = (g_physMemory.end - g_physMemory.start) / PAGE_SIZE;
	g_physMemory.freePages = g_physMemory.maxPages;
	SERIAL_SUCC("Physical mem start addr: 0x%08x | Len: %d mb | Max pages: %d", g_physMemory.start, g_physMemory.size / (1024 * 1024), g_physMemory.maxPages);
	SERIAL_SUCC("Physical mem end addr: 0x%08x", g_physMemory.end);

	memset(framesBitmap, 0, sizeof(framesBitmap));
}

uint32_t	getPhysicalAddr(uint32_t vAddr) {
	uint32_t pdIndex = PDE_INDEX(vAddr);
	uint32_t ptIndex = PTE_INDEX(vAddr);

	uint32_t* pageDir = (uint32_t*)REC_PAGE_DIR_ADD;
	if (!(pageDir[pdIndex] & FLAG_PAGE_PRESENT))
		return NULL; // No page table

	uint32_t* pageTab = (uint32_t*)PAGEDIR_ENTRY(pdIndex);
    // to do: check whether the PT entry is present.

    return (uint32_t)((pageTab[ptIndex] & ~0xFFF) + ((unsigned long)vAddr & 0xFFF));
}