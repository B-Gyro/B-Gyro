#include "memory/paging.h"
#include "memory/memory.h"
#include "klibc/memory.h"
#include "klibc/print.h"

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

void	*kmmap(size_t size) {
	size_t		nbrOfPages;
	uint32_t	pages;
	uint32_t	frame;
	
	nbrOfPages = size / PAGE_SIZE + !!(size % PAGE_SIZE);
	if (nbrOfPages > g_physMemory.freePages) // we don't have swap for now
		return NULL;

	if (getCurrentPageDir() != __BOOT_PAGE_DIRECTORY)
		setPageDir(__BOOT_PAGE_DIRECTORY);

	pages = allocPages(nbrOfPages, KERNEL_HEAP_START, 0);
	for (size_t i = 0; i < nbrOfPages; i++) {
		frame = allocFrame(0);
		mapPage(pages + i * PAGE_SIZE, frame, FLAG_PAGE_WRITE);
	}

	return (void *)(pages);
}

void	kunmap(uint32_t vaddr) {
	size_t		size;
	uint32_t	frame;

	size = freePages(vaddr, KERNEL_HEAP_START);
	for (size_t i = 0; i < size; i++) {
		frame = getPhysicalAddr(vaddr);
		freeFrame(frame);
		unmapPage(vaddr);
		vaddr += PAGE_SIZE;
	}
}