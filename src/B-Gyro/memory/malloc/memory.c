#include "memory/malloc/malloc.h"

void	printChunks(_ptr *chunks) {
	intptr_t	chunkEnd;
	_metaData	*chunk;

	while (chunks) {
		chunk = (_metaData *)(chunks - CHUNKS_OFFSET);
		if (chunk->isFree)
			VGA_PRINT(GREEN_ANSI);
		else
			VGA_PRINT(RED_ANSI);

		chunkEnd = (intptr_t)((char *)chunk + CHUNK_META_BLOCK_SIZE + chunk->size);
		VGA_PRINT("%010x - %010x : %d bytes", (intptr_t)((char *)chunk + CHUNK_META_BLOCK_SIZE), chunkEnd, chunk->size);

		// show fragmentation
		if (chunks->next && chunkEnd != (intptr_t)(chunks->next - CHUNKS_OFFSET)) {
			VGA_PRINT(GREY_ANSI);
			VGA_PRINT("%010x - %010x : %d bytes", chunkEnd, (chunks->next - CHUNKS_OFFSET), (chunks->next - CHUNKS_OFFSET) - chunkEnd);
		}

		chunks = chunks->next;
	}
	VGA_PRINT(RESET_ANSI);

}

void	printSpecificChunks(_ptr *chunks, bool isFree) {
	_metaData	*chunk;

	while (chunks) {
		chunk = (_metaData *)(chunks - CHUNKS_OFFSET);
		if (chunk->isFree == isFree)
			VGA_PRINT("%010x - %010x : %d bytes", (char *)chunk + CHUNK_META_BLOCK_SIZE, (char *)chunk + CHUNK_META_BLOCK_SIZE + chunk->size, chunk->size);
		chunks = chunks->next;
	}
}

void	printPageType(short type) {
	if (type == LARGE)
		VGA_PRINT("LARGE\n");
	else if (type == SMALL)
		VGA_PRINT("SMALL\n");
	else
		VGA_PRINT("TINY\n");
}


// ====================================================================================
void	show_free_mem(void){
	_ptr	*ptr = g_head;
	_page	*page;

	VGA_PRINT("FREE MEMORY=============================================\n");
	while (ptr) {
		page = (_page *)ptr;
		printPageType(page->type);
		printSpecificChunks((_ptr *)(((_metaData *)((char *)page + PAGE_META_BLOCK_SIZE))->ptr + CHUNKS_OFFSET), true);
		ptr = ptr->next;
	}
	VGA_PRINT("=============================================\n");
}

void	show_alloc_mem(void){
	_ptr	*ptr = g_head;
	_page	*page;

	VGA_PRINT("ALLOC MEMORY=============================================\n");
	while (ptr) {
		page = (_page *)ptr;
		printPageType(page->type);
		printSpecificChunks((_ptr *)(((_metaData *)((char *)page + PAGE_META_BLOCK_SIZE))->ptr + CHUNKS_OFFSET), false);
		ptr = ptr->next;
	}
	VGA_PRINT("=============================================\n");
}

void	show_mem(void){
	_ptr	*ptr = g_head;
	_page	*page;

	VGA_PRINT("MEMORY=============================================\n");
	while (ptr) {
		page = (_page *)ptr;
		printPageType(page->type);
		printChunks((_ptr *)(((_metaData *)((char *)page + PAGE_META_BLOCK_SIZE))->ptr + CHUNKS_OFFSET));
		ptr = ptr->next;
	}
	VGA_PRINT("=============================================\n");
}