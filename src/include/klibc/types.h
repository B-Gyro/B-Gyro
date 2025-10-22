#pragma once

# define uint32_t   unsigned int
# define uint16_t   unsigned short
# define uint8_t    unsigned char
# define int32_t    int
# define int16_t    short
# define int8_t     char
# define size_t   	uint32_t
# define ssize_t   	int32_t
# define uintptr_t	uint32_t
# define intptr_t	int32_t

# define bool	uint8_t
# define TRUE     1
# define FALSE    0
# define NULL     0

typedef struct uint64 {
	uint32_t low;
	uint32_t high;
} uint64_t;

				/* Number bases */
# define BIN_BASE "01"
# define DEC_BASE "0123456789"
# define HEX_BASE "0123456789ABCDEF"

				/* Bit manipulation */
# define BIT_IS_SET(FLAG, BIT) ((FLAG & (1 << BIT)) ? TRUE : FALSE)
# define BIT_SET(FLAG, BIT) FLAG = FLAG | (1 << BIT)
# define BIT_RESET(FLAG, BIT) FLAG = FLAG & ~(1 << BIT)

				/* Byte manipulation */
# define L8(VAL)	(VAL & 0xFF)			// get the low 8 bits of a 16 bit value
# define H8(VAL)	((VAL >> 8) & 0xFF)		// get the high 8 bits of a 16 bit value
# define L16(VAL)	(VAL & 0xFFFF)			// get the low 16 bits of a 32 bit value
# define H16(VAL)	((VAL >> 16) & 0xFFFF)	// get the high 16 bits of a 32 bit value

bool	isAlpha(uint8_t c);
bool	isDigit(uint8_t c);

bool	isPrintable(uint8_t c);

bool	isLowerCase(uint8_t c);
bool	isUpperCase(uint8_t c);

bool	toLowerCase(uint8_t c);
bool	toUpperCase(uint8_t c);
// get the size of a number in a given base
uint32_t	getNbrSize(uint32_t nbr, uint8_t baseLen);
