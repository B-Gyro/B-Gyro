#include "memory/malloc/malloc.h"

// 1st empty slot
_metaData	*getFirstEmptyBlock(size_t size, short type) {
	_ptr		*ptr = *getFreeChunksCollector(type);
	_metaData	*meta;

	while (ptr) {
		meta = (_metaData *)(ptr - FREE_CHUNKS_OFFSET);

		if (size <= meta->size) {
			return meta;
		}
		ptr = ptr->next;
	}
	return NULL;
}

// next empty slot
_metaData	*getNextEmptyBlock(size_t size, short type) {
	_ptr			*ptr;
	_metaData		*meta;
	static _ptr	*last = NULL;

	if (!last) {
		last = *getFreeChunksCollector(type);
	}
	
	ptr = last;
	while (last) {
		meta = (_metaData *)(last - FREE_CHUNKS_OFFSET);
		if (size <= meta->size) {
			last = last->next;
			return meta;
		}
		last = last->next;
	}

	last = *getFreeChunksCollector(type);
	while (last != ptr) {
		meta = (_metaData *)(last - FREE_CHUNKS_OFFSET);
		if (size <= meta->size) {
			last = last->next;
			return meta;
		}
		last = last->next;
	}
	return NULL;
}

// best empty slot
_metaData	*getBestEmptyBlock(size_t size, short type) {
	_ptr		*ptr = *getFreeChunksCollector(type);
	_metaData	*meta, *best;

	best = NULL;
	while (ptr) {
		meta = (_metaData *)(ptr - FREE_CHUNKS_OFFSET);
		if ((size <= meta->size) 
			&& (!best || (best->size > meta->size)))
				best = meta;
		ptr = ptr->next;
	}
	return best;
}

// worst empty slot
_metaData	*getWorstEmptyBlock(size_t size, short type) {
	_ptr		*ptr = *getFreeChunksCollector(type);
	_metaData	*meta, *worst;

	worst = NULL;
	while (ptr) {
		meta = (_metaData *)(ptr - FREE_CHUNKS_OFFSET);
		if ((size <= meta->size) 
			&& (!worst || (worst->size < meta->size)))
				worst = meta;
		ptr = ptr->next;
	}
	return worst;
}
