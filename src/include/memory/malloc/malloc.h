#ifndef	MALLOC_H
# define MALLOC_H

# include "chunks.h"

void	*malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);
void	*calloc(size_t nmemb, size_t size);

void	show_mem(void);

void	show_free_mem(void);
void	show_alloc_mem(void);
size_t	getNbrOfPages(size_t x);

#endif