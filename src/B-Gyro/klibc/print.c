# include "klibc/print.h"
# include "klibc/vargs.h"
# include "klibc/converts.h"
# include "klibc/math.h"

int16_t		g_appendingWidth = 0;
char		g_appendingChar = ' ';
uint32_t	floatPrecision = 5;
_buff		sprintfBuff;


static void	resetFormatingVariables(){
	g_appendingChar = ' ';
	g_appendingWidth = 0;
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
		g_appendingChar = '0';
		(*index)++;
	}
	g_appendingWidth = atoiS(fmtString + *index, &skippingLen);
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
		putChar(g_appendingChar);

	return (len >= 0 ? len : 0);
}

static uint32_t	printDSpecifier(putCharFnc putChar, int32_t nbr) {

	uint8_t isNegative;
	uint32_t	printedSize;

	isNegative = nbr < 0;
	if (isNegative){
		g_appendingWidth--;
		nbr *= -1;	
	}
	g_appendingChar = '0';
	printedSize = appendBeforePrinting(putChar, g_appendingWidth - getNbrSize(nbr, 10));
	if (isNegative)
		putChar('-');
	printedSize += printNumber(putChar, nbr, DEC_BASE, 10) + isNegative;
	return printedSize;
}

static uint32_t	printXSpecifier(putCharFnc putChar, uint32_t nbr){
	uint32_t	printedSize;

	printedSize = appendBeforePrinting(putChar, g_appendingWidth - getNbrSize(nbr, 16));
	printedSize += printNumber(putChar, nbr, HEX_BASE, 16);
	return printedSize;
}

static uint32_t	printUSpecifier(putCharFnc putChar, uint32_t nbr){
	uint32_t	printedSize;

	printedSize = appendBeforePrinting(putChar, g_appendingWidth - getNbrSize(nbr, 10));
	printedSize += printNumber(putChar, nbr, DEC_BASE, 10);
	return printedSize;
}

static uint32_t	printPSpecifier(putCharFnc putChar, uint32_t nbr){
	uint32_t	printedSize;

	g_appendingWidth -= 2;

	printedSize  = printSSpecifier(putChar, "0x");
	printedSize += appendBeforePrinting(putChar, g_appendingWidth - getNbrSize(nbr, 16));
	printedSize += printNumber(putChar, nbr, HEX_BASE, 16);
	return printedSize;
}

static uint32_t	printBSpecifier(putCharFnc putChar, uint32_t nbr){
	uint32_t	printedSize;

	printedSize = appendBeforePrinting(putChar, g_appendingWidth - getNbrSize(nbr, 2));
	printedSize += printNumber(putChar, nbr, BIN_BASE, 2);
	return printedSize;
}

static uint32_t printFSpecifier(putCharFnc putChar, double nbr){
	uint32_t	printedSize;
	int32_t		intPart;
	int32_t		decPart;

	printedSize = 0;
	if (nbr < 0){
		printedSize += 1;
		printedSize += putChar('-');
		nbr *= -1;
	}

	intPart = (uint32_t)nbr;

	// Calculate decimal part with proper scaling
    double remainder = nbr - (double)intPart;
    uint32_t scale = 1;
    
    // Scale up based on precision
    for (uint8_t i = 0; i < floatPrecision; i++) {
        scale *= 10;
    }

	decPart = remainder * scale + 0.5;
	printedSize += printDSpecifier(putChar, intPart);
	printedSize += putChar('.');
	printedSize += printDSpecifier(putChar, decPart);

	return printedSize;
}

static uint32_t	handlePrintSpecifier(putCharFnc putChar, varg_ptr *vptr, char *fmtString, uint32_t *index){		
	switch (fmtString[*index])
	{
		case 'c': return putChar(VARG_NEXT(*vptr, int32_t));
		case 's': return printSSpecifier(putChar, VARG_NEXT(*vptr, char *));
		case 'd': return printDSpecifier(putChar, VARG_NEXT(*vptr, int32_t));
		case 'u': return printUSpecifier(putChar, VARG_NEXT(*vptr, int32_t));
		case 'x': return printXSpecifier(putChar, VARG_NEXT(*vptr, uint32_t));
		case 'p': return printPSpecifier(putChar, VARG_NEXT(*vptr, uint32_t));
		case 'b': return printBSpecifier(putChar, VARG_NEXT(*vptr, uint32_t));
		case 'f': return printFSpecifier(putChar, VARG_NEXT_DOUBLE(*vptr));
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

uint8_t appendSprintfBuff(char c){
	if (!sprintfBuff.buff)
		SERIAL_ERR("sprintfBuff is not set properly !!\r\n", NULL);
	sprintfBuff.buff[sprintfBuff.index] = c;
	sprintfBuff.index++;
	return 1;
}

// used to set/reset the sprintfBuff:
void setSprintfBuff(char *buff) {
	if (sprintfBuff.buff)
		sprintfBuff.buff[sprintfBuff.index] = 0;
	sprintfBuff.buff = buff;
	sprintfBuff.index = 0;
}

void	printError(char *error){
	if (!error)
		return ;
	VGA_PRINT("%sError: %s\n%s", COLOR_RED, error, COLOR_RESET);
}