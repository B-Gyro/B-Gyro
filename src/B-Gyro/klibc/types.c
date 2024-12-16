#include "klibc/types.h"


bool	isAlpha(uint8_t c){
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

bool	isDigit(uint8_t c){
	return (c >= '0' && c <= '9');
}

bool	isPrintable(uint8_t c){
	return (c >= ' ' && c < 127);
}

bool	isLowerCase(uint8_t c){
	return (c >= 'a' && c <= 'z');
}
bool	isUpperCase(uint8_t c){
	return (c >= 'A' && c <= 'Z');
}

bool	toLowerCase(uint8_t c){
	if (isUpperCase(c))
		c += 32;

	return c;
}
bool	toUpperCase(uint8_t c){
	if (isLowerCase(c))
		c -= 32;

	return c;
}

uint32_t	getNbrSize(uint32_t nbr, uint8_t baseLen){
	uint32_t i = 0;

	do{
		nbr /= baseLen, i++;
	}while (nbr);

	return i;
}
