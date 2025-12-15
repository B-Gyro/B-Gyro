#include "memory/malloc/malloc.h"

// _ptr	**getChunksCollector(_ptr *chunk) {
// 	_page	*page;
// 	while (chunk->previous)
// 		chunk = chunk->previous;
	
// 	page = (_page *)(((char *)(chunk - CHUNKS_OFFSET)) - PAGE_META_BLOCK_SIZE);
// 	return (&(page->chunks));
// }

size_t	getNbrOfPages(size_t x){
	static size_t n = 0;

	n += x;
	return n;
}

// _ptr	**getGarbageCollector(void) {
// 	static _ptr	*garbageCollector;

// 	return &garbageCollector;
// }

_ptr	**getFreeChunksCollector(short type) {
	static _ptr	*tinyChunksCollector, 
				*smallChunksCollector, 
				*largeChunksCollector;

	if (type == TINY)
		return &tinyChunksCollector;
	else if (type == SMALL)
		return &smallChunksCollector;
	else
		return &largeChunksCollector;
}