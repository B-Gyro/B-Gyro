#include "memory/paging.h"
#include "memory/memory.h"
#include "klibc/memory.h"
#include "klibc/print.h"

void	*kmmap(size_t size) {
	size_t		nbrOfPages;
	uint32_t	pages;
	uint32_t	frame;
	
	nbrOfPages = size / PAGE_SIZE + !!(size % PAGE_SIZE);
	if (!size || nbrOfPages > g_physMemory.freePages) // we don't have swap for now
		return NULL;

	if (getCurrentPageDir() != __BOOT_PAGE_DIRECTORY)
		setPageDir(__BOOT_PAGE_DIRECTORY);

	pages = allocPages(nbrOfPages, KERNEL_HEAP_START, 0);
	if (!pages)
		return NULL;

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
		frame = getFrameAddr(vaddr);
		freeFrame(frame);
		unmapPage(vaddr);
		vaddr += PAGE_SIZE;
	}
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