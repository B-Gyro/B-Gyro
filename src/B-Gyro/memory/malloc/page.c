#include "memory/malloc/chunks.h"
#include "memory/paging.h"

_ptr	*g_head = NULL;

size_t	getNbrOfPages(size_t x);

short	getPageType(size_t size) {
	if (size <= TINY_ALLOC_SIZE) 
		return TINY;
	else if (size <= SMALL_ALLOC_SIZE)
		return SMALL;
	return LARGE;
}

size_t	getPageSize(size_t size, short type) {
	if (type == TINY)
		return TINY_PAGE_SIZE;
	else if (type == SMALL)
		return SMALL_PAGE_SIZE;

	size += PAGE_META_BLOCK_SIZE + CHUNK_META_BLOCK_SIZE;
	size_t mod = size % PAGE_SIZE;
	return (size - mod + (mod ? PAGE_SIZE : 0));
}

void	*newVMPage(size_t pageSize) {
	void	*vmPage;

	vmPage = kmmap(pageSize);
	
	if (vmPage == MAP_FAILED) {
		VGA_PRINT(RED_ANSI);
		VGA_PRINT("ERROR: MMAP FAILED\n");
		VGA_PRINT(RESET_ANSI);
		return NULL;
	}

	getNbrOfPages(pageSize / PAGE_SIZE);

	return vmPage;
}

_metaData	*newPage(size_t size, short type) {
	_page	*page;
	void	*vmPage;
	size_t	pageSize;
	_metaData	*meta;

	pageSize = getPageSize(size, type);
	vmPage = newVMPage(pageSize);
	if (!vmPage)
		return NULL;
	// init page
	page = (_page *)vmPage;
	meta = (_metaData *)((char *)vmPage + PAGE_META_BLOCK_SIZE);

	page->ptr.next = NULL;
	page->ptr.previous = NULL;
	// page->chunks = meta->ptr + CHUNKS_OFFSET;
	page->type = type;

	// insert in pages list 
	insertNode(&g_head, &page->ptr);

	// init big free chunk
	initChunk((void *)((char *)vmPage + PAGE_META_BLOCK_SIZE), pageSize - PAGE_META_BLOCK_SIZE, type);
	return (meta);
}

void	deletePage(_metaData *meta, short type) {
	_page	*page;
	_ptr	*ptr;
	size_t	size = 0;

	ptr = meta->ptr + CHUNKS_OFFSET;
	while (ptr->previous) {
		ptr = ptr->previous;
	}

	page = (_page *)((char *)(ptr - CHUNKS_OFFSET) - PAGE_META_BLOCK_SIZE);
	
	ptr = (_ptr *)(((_metaData *)((char *)page + PAGE_META_BLOCK_SIZE))->ptr + CHUNKS_OFFSET);
	while (ptr) {
		meta = (_metaData *)(ptr - CHUNKS_OFFSET);
		size += meta->size + CHUNK_META_BLOCK_SIZE;
		if (meta->isFree)
		// 	deleteNode(getGarbageCollector(), meta->ptr + GARBAGE_COLLECTOR_OFFSET);
		// else
			deleteNode(getFreeChunksCollector(type), meta->ptr + FREE_CHUNKS_OFFSET);

		ptr = ptr->next;
	}
	
	size += PAGE_META_BLOCK_SIZE;
	deleteNode(&g_head, (void *)page);
	kunmap((uint32_t)page);
	// if (kunmap((uint32_t)page)) {
	// 	VGA_PRINT(RED_ANSI);
	// 	VGA_PRINT("ERROR: KUNMAP FAILED\n");
	// 	VGA_PRINT(RESET_ANSI);
	// }
	getNbrOfPages(-(size / PAGE_SIZE));
}

// delete page if there's other pages from same type that has free spaces
void	cleanPages(_metaData *meta, short type) {
	_ptr	*freeChunksCollector = *getFreeChunksCollector(type);

	// to do: redo
	if (!freeChunksCollector 
		|| !freeChunksCollector->next)
		return ;
	deletePage(meta, type);
}