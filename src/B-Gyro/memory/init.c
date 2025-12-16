#include "memory/memory.h"
#include "memory/paging.h"
#include "klibc/memory.h"
#include "klibc/print.h"


// physical =================================================
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

// virtual =================================================

# define	PAGES_BITMAP_SIZE (KERNEL_PAGES_NBR / (32 / 2))

static _pageDirectory pageDirs[PAGES_DIRS_NBR] __attribute__((aligned(4096)));
static bool pageDirUsed[PAGES_DIRS_NBR];
extern uint32_t	pagesBitmap[PAGES_BITMAP_SIZE]; 

static void	clearPageDir() {
	// PDE 0 is special because it maps the lowest 4 MB, 
	// which is often used by the bootloader or BIOS.

	// After the kernel switches to higher-half mapping, 
	// the first 4 MB is no longer needed for identity mapping.

	// If we left it mapped, we could accidentally access physical memory at 0x0, 
	// which is often reserved for hardware or the bootloader.
	
    __BOOT_PAGE_DIRECTORY[0] = 0;
    invalidate(0);
}

static void	EnableRecursivePaging() {
	// https://wiki.osdev.org/User:Neon/Recursive_Paging
	// https://medium.com/@connorstack/recursive-page-tables-ad1e03b20a85
	__BOOT_PAGE_DIRECTORY[1023] = ((uint32_t) __BOOT_PAGE_DIRECTORY - KERNEL_START) | FLAG_PAGE_PRESENT | FLAG_PAGE_WRITE;
	invalidate(REC_PAGE_DIR_ADD);
}

void	initVirtualMemory() {
	clearPageDir();
	EnableRecursivePaging();

	memset(pagesBitmap, 0, KERNEL_PAGES_NBR / UINT8_SIZE);
	memset(pageDirs, 0, 1024 * PAGES_DIRS_NBR);
	memset(pageDirUsed, 0, PAGES_DIRS_NBR);
}
