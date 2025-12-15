#include "memory/malloc/malloc.h"

void	*malloc(size_t size) {
	// VGA_PRINT("-");
	if (!size)
		return NULL;
	void	*res = allocChunk(size);
	return res;
}

void	free(void *ptr) {
	if (!ptr || !isAllocated((_ptr *)(ptr)))
		return;

	freeChunk(ptr);
}

void	*calloc(size_t nmemb, size_t size) {
	void	*ptr;

	size *= nmemb;
	ptr = malloc(size);
	memset(ptr, 0, size);

	return ptr;
}

void	*realloc(void *ptr, size_t size) {
	_metaData *meta;

	if (!ptr)
		return (malloc(size));

	meta = (_metaData *)((char *)ptr - CHUNK_META_BLOCK_SIZE);

	if (!size || !isAllocated((_ptr *)(ptr)))
		return NULL;

	// to do: protect
	if (meta->size == size)
		return ptr;

	void	*res = malloc(size);
	memcpy(res, ptr, (size > meta->size ? meta->size : size));
	freeChunk(ptr);
	return (res);

}