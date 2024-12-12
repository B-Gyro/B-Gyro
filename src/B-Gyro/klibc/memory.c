#include "klibc/memory.h"

void     bzero(void *s, size_t n){
	memset(s, 0, n);
}

void    *memset(void *s, int c, size_t n){
	uint8_t *ptr;

	ptr = (uint8_t *)s;

	for (size_t i = 0; i < n; i++)
		ptr[i] = c;

	return s;
}

void    *memchr(const void *s, int c, size_t n){
	const char *constS;

	constS = (const char *)s;
	for (size_t i = 0; i < n; i++){
		if (constS[i] == c)
			return (void *)&constS[i];
	}

	return NULL;
}
void    *memcpy(void *dest, const void *src, size_t n){
	uint8_t	*ptrDest, *ptrSrc;

	ptrDest = (uint8_t *)dest;
	ptrSrc = (uint8_t *)src;

	for (size_t i = 0; i < n; i++)
		ptrDest[i] = ptrSrc[i];

	return dest;
}

void    *memmove(void *dest, const void *src, size_t n){
	uint8_t	*ptrDest, *ptrSrc;

	ptrDest = (uint8_t *)dest;
	ptrSrc = (uint8_t *)src;

	if (ptrDest < ptrSrc){
		for (size_t i = 0; i < n; i++)
			ptrDest[i] = ptrSrc[i];
	}
	else{
		for (size_t i = n; i > 0; i--)
			ptrDest[i - 1] = ptrSrc[i - 1];
	}

	return dest;
}

int      memcmp(const void *s1, const void *s2, size_t n){
	const uint8_t	*ptrS1, *ptrS2;

	ptrS1 = (const uint8_t *)s1;
	ptrS2 = (const uint8_t *)s2;

	for (size_t i = 0; i < n; i++){
		if (ptrS1[i] != ptrS2[i])
			return ptrS1[i] - ptrS2[i];
	}

	return 0;
}
