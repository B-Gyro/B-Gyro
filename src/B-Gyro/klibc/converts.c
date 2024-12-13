#include "klibc/strings.h"
# include "klibc/converts.h"
# include "arch/i386/ports/serialCommunication/serialIO.h"
# include "klibc/print.h"

uint32_t	printD(putCharFnc putChar, int32_t nbr);

static size_t skipSpaces(const char *s){
	size_t index;

	index = 0;
	while (s[index] == ' ')
		index++;
	return index;
}

// a safer version of atoi
ssize_t atoiS(const char *s, size_t *index){
	ssize_t	nbr;
	int8_t	sign;
	size_t	fakeIndex;

	nbr = 0;
	sign = 1;
	if (!index)	// if index is NULL, we use a fake index
		index = &fakeIndex;

	*index = skipSpaces(s);
	if (s[*index] == '-' || s[*index] == '+'){
		sign = s[*index] == '-' ? -1 : 1;
		(*index)++;
	}
	while (isDigit(s[*index])){
		nbr = nbr * 10 + (s[*index] - 48);
		(*index)++;
	}

	return nbr * sign;
}

// convert a string to an integer
size_t	uatoiS(const char *s, size_t *index){
	size_t	nbr, fakeIndex;

	if (!index)	// if index is NULL, we use a fake index
		index = &fakeIndex;

	*index = nbr = 0;
	// skip spaces
	*index = skipSpaces(s);
	while (isDigit(s[*index]))
	{
		nbr = nbr * 10 + (s[*index] - 48);
		(*index)++;
	}

	return nbr;
}

// convert a hex string to an integer
size_t	aHextoiS(const char *s, size_t *index){
	ssize_t	i;
	size_t	nbr, fakeIndex;

	if (!index)
		index = &fakeIndex;
	// skip spaces
	*index = skipSpaces(s);

	// skip 0x if present
	if (s[*index] == '0' && s[*index + 1] == 'x')
		*index += 2;

	while (s[*index]) {
		i = strchrInd(HEX_BASE, toUpperCase(s[*index]));
		if (i == -1)
			break;
		nbr = (nbr * 16) + i; // update number
		*index += 1;
	}
	return nbr;
}

size_t	uatoi(const char *s, size_t *index){
	size_t	nbr;

	if (!index)
		return (0);

	nbr = 0;

	while (isDigit(s[*index])) {
		nbr = nbr * 10 + (s[*index] - 48);
		(*index)++;
	}

	return nbr;
}
