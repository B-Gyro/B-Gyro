#pragma once

# include "klibc/types.h"
# include "memory/memory.h"

# define FLAG_PAGE_PRESENT			(1 << 0)
# define FLAG_PAGE_WRITE			(1 << 1)
# define FLAG_PAGE_USER_SUPERVISOR	(1 << 2)
# define FLAG_PAGE_WRITE_THROUGH	(1 << 3)
# define FLAG_PAGE_CACHE_DISBLED	(1 << 4)
# define FLAG_PAGE_ACCESSED			(1 << 5)
// # define FLAG_PAGE_AVAILABLE		(1 << 6)
# define FLAG_PAGE_SIZE				(1 << 7)
// # define FLAG_PAGE_AVAILABLE		(1 << 8)
# define FLAG_PAGE_OWNER			(1 << 9)


# define PAGE_SIZE	4096
# define PAGES_DIRS_NBR 200 // change later

# define REC_PAGE_DIR_ADD	0xFFFFF000
# define PAGEDIR_ENTRY(i)	((uint32_t *)0xFFC00000) + (i << 10)

// vaddr >> 22 → upper 10 bits → index into page directory.
// (vaddr >> 12) & 0x3FF → next 10 bits → index into page table.
// vaddr & 0xFFF → offset inside the page.
# define PDE_INDEX(vaddr) (vaddr >> 22)
# define PTE_INDEX(vaddr) ((vaddr >> 12) & 0x3FF)

# define THEORICAL_FRAMES_NUMBER (0x100000000 / PAGE_SIZE) // max = 4G
# define REAL_FRAMES_NUMBER		 (g_physMemory.maxPages)

# define PAGES_NUMBER			THEORICAL_FRAMES_NUMBER

# define ALIGN_UP(n)	(n + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
# define ALIGN_DOWN(n)	(n & ~4095)

extern uint32_t __BOOT_PAGE_DIRECTORY[1024];
extern uint8_t	framesBitmap[THEORICAL_FRAMES_NUMBER / UINT8_SIZE]; // frame per bit
extern uint32_t	pagesBitmap[KERNEL_PAGES_NBR / (32 / 2)]; 

typedef uint32_t _pageTableEntry;

typedef struct pageDirectory {
	_pageTableEntry	pageTable[1024];
} _pageDirectory;

void		initVirtualMemory();
uint32_t	allocPages(uint32_t numberOfPages, uint32_t heapStart, uint32_t end);
size_t		freePages(uint32_t ptr, uint32_t heapStart);

void		mapPage(uint32_t virtAddr, uint32_t physAddr, uint32_t flags);
void		unmapPage(uint32_t virtAddr);

uint32_t	getPhysicalAddr(uint32_t vAddr);
