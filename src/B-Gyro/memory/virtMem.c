#include "memory/paging.h"
#include "memory/memory.h"
#include "klibc/memory.h"
#include "klibc/print.h"

# define	PAGES_BITMAP_SIZE (KERNEL_PAGES_NBR / (32 / 2))

uint32_t	pagesBitmap[PAGES_BITMAP_SIZE]; 

static _pageDirectory pageDirs[PAGES_DIRS_NBR] __attribute__((aligned(4096)));
static bool pageDirUsed[PAGES_DIRS_NBR];

void invalidate(uint32_t addr) {
	asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

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

# define PAGE_PER_UINT32			16

# define FREE_PAGE					0b00
# define ALLOCATED_PAGE_FIRST		0b01
# define ALLOCATED_PAGE_WITH_NEXT	0b10
# define ALLOCATED_PAGE_NO_NEXT		0b11
# define GET_PAGE_FLAG				0b11

// next free page algo | call it with end = 0
uint32_t	allocPages(uint32_t numberOfPages, uint32_t heapStart, uint32_t end){
	static uint32_t	last;
	uint32_t		x = 0, index;
	short			indexBit;
	uint32_t		addr;

	end = (!end ? REAL_FRAMES_NUMBER / PAGE_PER_UINT32 : end);

	for (uint32_t i = last; i < end; i++) {
		if (pagesBitmap[i] != 0xFFFFFFFF) {
			for (short bit = PAGE_PER_UINT32 - 1; bit >= 0; bit--) {
				if (!(pagesBitmap[i] & (0b11 << (bit * 2)))) {
					if (!x) {
						index = i;
						indexBit = bit;
					}
					x++;
					if (x == numberOfPages) {
						addr = (uint32_t)(heapStart + (index * PAGE_PER_UINT32 + (PAGE_PER_UINT32 - 1 - indexBit))  * PAGE_SIZE);
						if (numberOfPages == 1)
							pagesBitmap[index] |= (ALLOCATED_PAGE_NO_NEXT << (indexBit * 2));
						else {
							pagesBitmap[index] |= (ALLOCATED_PAGE_FIRST << (indexBit-- * 2));
							x--;
							for (; index <= i; index++) {
								for (; indexBit >= ((index == i) ? bit : 0); indexBit--) {
									x--;
									pagesBitmap[index] |= ((x ? ALLOCATED_PAGE_WITH_NEXT : ALLOCATED_PAGE_NO_NEXT) << (indexBit * 2));
								}
								indexBit = PAGE_PER_UINT32 - 1;
							}
						}
						last = i + !bit;
						return (addr);
					}
				}
				else 
					x = 0;
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

size_t	freePages(uint32_t ptr, uint32_t heapStart){
	short		bit;
	uint32_t	index;
	short		pageFlag;
	size_t		x = 0;

	index = (uint32_t)((ptr - heapStart) / PAGE_SIZE);
	bit = PAGE_PER_UINT32 - 1 - (index % PAGE_PER_UINT32);
	index /= PAGE_PER_UINT32;

	pageFlag = (pagesBitmap[index] >> (bit * 2)) & GET_PAGE_FLAG;

	if (pageFlag == FREE_PAGE || pageFlag == ALLOCATED_PAGE_WITH_NEXT){
		return 0; // error invalid ptr (IT'S FREE OR NOT FIRST BLOCK)
	}
	
	if (pageFlag == ALLOCATED_PAGE_NO_NEXT) {
		pageFlag = (pagesBitmap[((!bit && index) ? index - 1 : index)] >> (((!bit && index) ? PAGE_PER_UINT32 - 1 : bit - 1) * 2)) & GET_PAGE_FLAG;
		if (pageFlag == ALLOCATED_PAGE_WITH_NEXT)
			return 0; // error invalid ptr (IT'S NOT FIRST BLOCK)
	}

	while (index < PAGES_BITMAP_SIZE) {
		for (; bit >= 0; bit--) {
			pageFlag = (pagesBitmap[index] >> (bit * 2)) & GET_PAGE_FLAG;
			pagesBitmap[index] &= ~(0b11 << (bit * 2)); // FREE PAGE
			x++;
			if (pageFlag == ALLOCATED_PAGE_NO_NEXT)
				return x;
		}
		bit = PAGE_PER_UINT32 - 1;
		index++;
	}
	return 0;
}

void mapPage(uint32_t virtAddr, uint32_t physAddr, uint32_t flags){
	uint32_t pdIndex = PDE_INDEX(virtAddr);
	uint32_t ptIndex = PTE_INDEX(virtAddr);

	uint32_t* pageDir = (uint32_t*)REC_PAGE_DIR_ADD;
	uint32_t* pageTab = (uint32_t*)PAGEDIR_ENTRY(pdIndex);

	if (!(pageDir[pdIndex] & FLAG_PAGE_PRESENT)){
		uint32_t frame = allocFrame(0);
		pageDir[pdIndex] = frame | FLAG_PAGE_PRESENT | FLAG_PAGE_WRITE | FLAG_PAGE_OWNER | flags;
		invalidate(virtAddr);

		for (uint32_t i = 0; i < 1024; i++){
			pageTab[i] = 0;
		}
	}

	pageTab[ptIndex] = physAddr | FLAG_PAGE_PRESENT | flags;
	invalidate(virtAddr);
}

void unmapPage(uint32_t virtAddr){
	uint32_t pdIndex = PDE_INDEX(virtAddr);
	uint32_t ptIndex = PTE_INDEX(virtAddr);

	uint32_t* pageDir = (uint32_t*)REC_PAGE_DIR_ADD;

	if (!(pageDir[pdIndex] & FLAG_PAGE_PRESENT))
		return ; // No page table

	uint32_t* pageTab = (uint32_t*)PAGEDIR_ENTRY(pdIndex);
	pageTab[ptIndex] = 0;
	
	invalidate(virtAddr);
}