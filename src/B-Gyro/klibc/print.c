# include "klibc/print.h"
# include "klibc/vargs.h"
# include "klibc/converts.h"

int16_t appendingWidth = 0;
char	AppendingChar = ' ';
_buff	newAprintBuff;


static void	resetFormatingVariables(){
	AppendingChar = ' ';
	appendingWidth = 0;
}


static size_t		printNumber(putCharFnc putChar, size_t nbr, char *base, uint8_t	baseLen){
	int printedSize = 0;

	if (nbr >= baseLen)
		printedSize = printNumber(putChar, nbr / baseLen, base, baseLen);
	putChar(base[nbr % baseLen]);
	return (++printedSize);
}

static void	ConsumeAppendingSpecifier(char *fmtString, uint32_t *index){

	uint32_t	skippingLen;

	if (fmtString[*index] == '0'){
		AppendingChar = '0';
		(*index)++;
	}
	appendingWidth = atoiS(fmtString + *index, &skippingLen);
	*index += skippingLen;
	return ;
}

static uint32_t	printSSpecifier(putCharFnc putChar, char *s){
	uint32_t	i;

	i = 0;
	if (!s)
		return printSSpecifier(putChar, "(NULL)");
	while(s[i]){
		putChar(s[i]);
		i++;
	}
	return i;
}

static uint32_t	appendBeforePrinting(putCharFnc putChar, int16_t len){
	for (int16_t i = 0; i < len; i++)
		putChar(AppendingChar);

	return (len >= 0 ? len : 0);
}

static uint32_t	printDSpecifier(putCharFnc putChar, int32_t nbr) {

	uint8_t isNegative;
	uint32_t	printedSize;

	isNegative = nbr < 0;
	if (isNegative){
		appendingWidth--;
		nbr *= -1;	
	}
	printedSize = appendBeforePrinting(putChar, appendingWidth - getNbrSize(nbr, 10));
	if (isNegative)
		putChar('-');
	printedSize += printNumber(putChar, nbr, DEC_BASE, 10) + isNegative;
	return printedSize;
}

static uint32_t	printXSpecifier(putCharFnc putChar, uint32_t nbr){
	uint32_t	printedSize;

	printedSize = appendBeforePrinting(putChar, appendingWidth - getNbrSize(nbr, 16));
	printedSize += printNumber(putChar, nbr, HEX_BASE, 16);
	return printedSize;
}

static uint32_t	printPSpecifier(putCharFnc putChar, uint32_t nbr){
	uint32_t	printedSize;

	appendingWidth -= 2;

	printedSize  = printSSpecifier(putChar, "0x");
	printedSize += appendBeforePrinting(putChar, appendingWidth - getNbrSize(nbr, 16));
	printedSize += printNumber(putChar, nbr, HEX_BASE, 16);
	return printedSize;
}

static uint32_t	printBSpecifier(putCharFnc putChar, uint32_t nbr){
	uint32_t	printedSize;

	printedSize = appendBeforePrinting(putChar, appendingWidth - getNbrSize(nbr, 2));
	printedSize += printNumber(putChar, nbr, BIN_BASE, 2);
	return printedSize;
}

static uint32_t	handlePrintSpecifier(putCharFnc putChar, varg_ptr *vptr, char *fmtString, uint32_t *index){		
	switch (fmtString[*index])
	{
		case 'c': return putChar(VARG_NEXT(*vptr, int32_t));
		case 's': return printSSpecifier(putChar, VARG_NEXT(*vptr, char *));
		case 'd': return printDSpecifier(putChar, VARG_NEXT(*vptr, int32_t));
		case 'x': return printXSpecifier(putChar, VARG_NEXT(*vptr, uint32_t));
		case 'p': return printPSpecifier(putChar, VARG_NEXT(*vptr, uint32_t));
		case 'b': return printBSpecifier(putChar, VARG_NEXT(*vptr, uint32_t));
		default:
			break;
	}

	if (isDigit(fmtString[*index])){
		ConsumeAppendingSpecifier(fmtString, index);
		return handlePrintSpecifier(putChar, vptr, fmtString, index);
	}
	return putChar(fmtString[*index]);
}

uint32_t	print(putCharFnc putChar, char *fmtString, ...){
	varg_ptr	vptr;
	uint32_t	i, printedSize;

	VARG_INIT(vptr, fmtString);
	i = printedSize = 0;
	while (fmtString[i]) {
		resetFormatingVariables();
		if (fmtString[i] == '%'){
			i++;
			printedSize += handlePrintSpecifier(putChar, &vptr, fmtString, &i);
		}
		else
			printedSize += putChar(fmtString[i]);
		i++;
	}
	return printedSize;
}