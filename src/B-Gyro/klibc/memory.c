#include "klibc/memory.h"
#include "terminal/_data.h"

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

// size of uint16_t (ex: _vgaCell)
void	bigBzero(void *address, uint32_t size){
	uint32_t	*ptr;
	
	ptr = (uint32_t *)address;
	for (uint32_t i = 0; i < (size / 2); i++)
		ptr[i] = 0;
}

// for keyboard buffer / history
size_t	safeStrcpy(unsigned char *dest, const unsigned char *src, size_t maxSize) {
	size_t	len;

	for (len = 0; len < maxSize; len++) {
		dest[len] = src[len];
		if (!src[len] || (src[len] == '\n'))
			break;
	}
	if (dest[len] && (maxSize != MAX_KEYBOARD_BUFFER))
		dest[len] = 0;
	return (len);
}

void	dumpMemory(uint8_t *startAddr, size_t size){
    uint8_t *endAddr = startAddr + size;
	char	*color;

    while (startAddr < endAddr)
    {
        // print address
        VGA_PRINT("%08p ", startAddr);
        SERIAL_PRINT("%08p ", startAddr);
        // print hex values
        for (uint8_t i = 0; i < 16; i++) {
            color = (startAddr[i] == 0) ? COLOR_WHITE : isPrintable(startAddr[i]) ? COLOR_GREEN : COLOR_RED;
            VGA_PRINT("%s%02x\033[0m", color, startAddr[i]);
            SERIAL_PRINT("%s%02x\033[0m", color, startAddr[i]);
            if (i % 2)
                VGA_PRINT(" ", NULL), SERIAL_PRINT(" ", NULL);
        }
        VGA_PRINT("  ", NULL), SERIAL_PRINT("  ", NULL);
        // print ascii values
        for (uint8_t i = 0; i < 16; i++) {
            color = (startAddr[i] == 0) ? COLOR_WHITE : isPrintable(startAddr[i]) ? COLOR_GREEN : COLOR_RED;
            VGA_PRINT("%s%c\033[0m", color, isPrintable(startAddr[i]) ? startAddr[i] : '.');
            SERIAL_PRINT("%s%c\033[0m", color, isPrintable(startAddr[i]) ? startAddr[i] : '.');
        }
        startAddr += 16;
        VGA_PRINT("\n\r", NULL), SERIAL_PRINT("\n\r", NULL);
    }
}