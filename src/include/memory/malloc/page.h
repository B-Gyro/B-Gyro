#ifndef	PAGE_H
# define PAGE_H

# include "klibc/listUtils.h"
# include "memory/paging.h"

# define TINY_ALLOC_SIZE	1024
# define SMALL_ALLOC_SIZE	1024 * 32

# define MALLOC_ALIGNMENT	16 // sizeof(size_t) * 2
# define ALIGN_MALLOC(n)	(n + (MALLOC_ALIGNMENT - 1)) & ~(MALLOC_ALIGNMENT - 1)

# define TINY_PAGE_SIZE		(size_t)ALIGN_UP((TINY_ALLOC_SIZE + CHUNK_META_BLOCK_SIZE) * 100 + PAGE_META_BLOCK_SIZE)	
# define SMALL_PAGE_SIZE	(size_t)ALIGN_UP((SMALL_ALLOC_SIZE + CHUNK_META_BLOCK_SIZE) * 100 + PAGE_META_BLOCK_SIZE)	

extern _ptr	*g_head;
typedef struct metaData _metaData;

// typedef enum type {
# define TINY	0
# define SMALL	1
# define LARGE	2
// } short;

typedef struct page {
	_ptr	ptr;
	short	type;
	char	pad[8];
} _page;

# define PAGE_META_BLOCK_SIZE	sizeof(_page)

void		*newVMPage(size_t size);
void		deletePage(_metaData *meta, short type);
void		cleanPages(_metaData *meta, short type);
_metaData	*newPage(size_t size, short type);

void	printPages();
short	getPageType(size_t size);

#endif
