#include "memory/malloc/chunks.h"

void	mergeChunks(_ptr *chunk1, _ptr *chunk2) {
	_metaData	*meta1, *meta2;

	meta1 = (_metaData *)((char *)chunk1 - CHUNKS_OFFSET);
	meta2 = (_metaData *)((char *)chunk2 - CHUNKS_OFFSET);

	meta1->size += meta2->size + CHUNK_META_BLOCK_SIZE;


	if (chunk2->next)
		chunk2->next->previous = chunk1;
	chunk1->next = chunk2->next;
}

void	splitChunk(_ptr *chunk, size_t size, short type) {
	_metaData	*meta, *next;
	size_t		alignedSize = ALIGN_MALLOC(size);

	meta = (_metaData *)((char *)chunk - CHUNKS_OFFSET);
	if (meta->size > (CHUNK_META_BLOCK_SIZE + alignedSize)) {
		next = (_metaData *)((char *)meta + CHUNK_META_BLOCK_SIZE + alignedSize);
		memset(next->ptr, 0, sizeof(_ptr) * 3);
		next->size = meta->size - alignedSize - CHUNK_META_BLOCK_SIZE;
		next->isFree = true;
		insertNode(getFreeChunksCollector(type), next->ptr + FREE_CHUNKS_OFFSET);

		next->ptr[CHUNKS_OFFSET].next = chunk->next;
		next->ptr[CHUNKS_OFFSET].previous = chunk;
		chunk->next = next->ptr + CHUNKS_OFFSET;
	}
	meta->size = size;
}

void	initChunk(void *block, size_t size, short type) {
	_metaData	*meta;

	meta = (_metaData *)block;
	memset(meta->ptr, 0, sizeof(meta->ptr));
	meta->size = size - CHUNK_META_BLOCK_SIZE;
	meta->isFree = true;
	insertNode(getFreeChunksCollector(type), meta->ptr + FREE_CHUNKS_OFFSET);
}

bool	isAllocated(void *toFind) {
	_ptr	*ptr = g_head;
	_metaData	*chunks;

	while (ptr) {
		if (ptr < (_ptr *)toFind) {
			chunks = (_metaData *)((char *)ptr + PAGE_META_BLOCK_SIZE);
			while (chunks)
			{
				if ((((char *)chunks + CHUNK_META_BLOCK_SIZE) == toFind)) {
					if (chunks->isFree)
						return false;
					return true;
				}
				chunks = (_metaData *)(chunks->ptr->next + CHUNKS_OFFSET);
			}
		}
		ptr = ptr->next;
	}
	return false;
}

// bool	isAllocated(void *toFind) {
	// _ptr	*ptr = *getGarbageCollector();

	// while (ptr) {
	// 	void *tmp = ((_ptr *)((char *)(ptr - GARBAGE_COLLECTOR_OFFSET) + CHUNK_META_BLOCK_SIZE));

	// 	if (tmp == toFind)
	// 		return true;
	// 	ptr = ptr->next;
	// }
// 	return false;
// }

void	*allocChunk(size_t size) {
	_metaData	*meta;
	size_t		alignedSize = ALIGN_MALLOC(size);
	short		type = getPageType(size);

	if (type == LARGE)
		meta = newPage(size, type);
	else {
		meta = MALLOC_ALGO(alignedSize, type);
		if (!meta)
			meta = newPage(alignedSize, type);
	}

	if (!meta) 
		return NULL;
	
	splitChunk(meta->ptr + CHUNKS_OFFSET, size, type);
	meta->isFree = false;
	deleteNode(getFreeChunksCollector(type), meta->ptr + FREE_CHUNKS_OFFSET);
	// insertNode(getGarbageCollector(), meta->ptr + GARBAGE_COLLECTOR_OFFSET);
	return ((void *)((char *)meta + CHUNK_META_BLOCK_SIZE));
}

void	freeChunk(void *ptr) {
	_ptr		*current, *prv, *next;
	size_t		frag;
	short		type;
	_metaData	*meta;

	meta = (_metaData *)((char *)ptr - CHUNK_META_BLOCK_SIZE);
	type = getPageType(meta->size);
	
	if (meta->isFree)
		return;

	if (type == LARGE) {
		// deleteNode(getGarbageCollector(), meta->ptr + GARBAGE_COLLECTOR_OFFSET);
		deletePage(meta, type);
		return ;
	}

	// deleteNode(getGarbageCollector(), meta->ptr + GARBAGE_COLLECTOR_OFFSET);

	meta->isFree = true;

	current = meta->ptr + CHUNKS_OFFSET;
	next = current->next;
	prv = current->previous;

	if (next) {
		// check for fragmentation after the block
		frag = ((char *)(next - CHUNKS_OFFSET) - \
			((char *)meta->ptr + CHUNK_META_BLOCK_SIZE + meta->size));
		if (frag)
			meta->size += frag;

		if(((_metaData *)(next - CHUNKS_OFFSET))->isFree) {
			deleteNode(getFreeChunksCollector(type), next - CHUNKS_OFFSET + FREE_CHUNKS_OFFSET);
			mergeChunks(current, next);
			next = current->next;
		}
	}


	if (prv) {
		_metaData *prvMeta = ((_metaData *)(prv - CHUNKS_OFFSET));

		frag = (char *)(meta) - \
			 ((char *)(prvMeta) + CHUNK_META_BLOCK_SIZE + prvMeta->size);
		
		frag = (frag / MALLOC_ALIGNMENT) * MALLOC_ALIGNMENT;
		
		if (prvMeta->isFree) {
			mergeChunks(prv, current);
			meta->size += frag;
			prv = prv->previous;
		}
		else {
			if (frag) {
				meta->size += frag;
				memcpy((void *)((char *)meta - frag), (void *)meta, sizeof(_metaData));
				meta = (_metaData *)((char *)meta - frag);
				prv->next = meta->ptr + CHUNKS_OFFSET;
				if (next)
					next->previous = meta->ptr + CHUNKS_OFFSET;
			}
			insertNode(getFreeChunksCollector(type), meta->ptr + FREE_CHUNKS_OFFSET);
		}
	} else {
		insertNode(getFreeChunksCollector(type), meta->ptr + FREE_CHUNKS_OFFSET);
	}

	if (!next && !prv)
		cleanPages(meta, type);
}