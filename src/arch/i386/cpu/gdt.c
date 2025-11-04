# include "arch/i386/cpu/descriptorTables.h"
# include "klibc/memory.h"
# include "klibc/print.h"

_gdtEntry	g_gdtEntries[GDT_ENTRIES];
_gdtPtr		g_gdtPtr;

// load the GDT to the CPU
extern void	gdtLoader(_gdtPtr *g_gdtPtr);

static void	setGdtEntry(uint8_t index, uint32_t limit, uint32_t base, uint8_t accessByte, uint8_t flags){
	
	if (index >= GDT_ENTRIES){
		SERIAL_ERR("GDT index out of range !!", NULL);
		return ;
	}
	// define the size of the segment:
	g_gdtEntries[index].lowLimit = L16(limit);
	g_gdtEntries[index].highLimit = (limit >> 16) & 0x0F;	// limit at the last for bits
	// define the base address of the segment:
	g_gdtEntries[index].lowBase	= L16(base);
	g_gdtEntries[index].middleBase = (base >> 16) & 0xFF;
	g_gdtEntries[index].highBase = (base >> 24) & 0xFF;
	// define the access/flag bits:
	g_gdtEntries[index].flags = flags & 0xF;	// flags stored at the first 4 bits
	g_gdtEntries[index].accessByte = accessByte;
}

void	initGDT(){

	setGdtEntry(0, 0, 0, 0, 0);						// NULL Segment

	setGdtEntry(1, 0xFFFFF, 0x0, 0x9A, 0xCf);		// Kernel Code Segment
	setGdtEntry(2, 0xFFFFF, 0x0, 0x92, 0xCf);		// Kernel Data Segment
	setGdtEntry(3, 0xFFFFF, 0x0, 0x92, 0xCf);		// Kernel Stack Segment

	setGdtEntry(4, 0xFFFFF, 0x0, 0xFA, 0xCf);		// User Code Segment
	setGdtEntry(5, 0xFFFFF, 0x0, 0xF2, 0xCf);		// User Data Segment
	setGdtEntry(6, 0xFFFFF, 0x0, 0xF2, 0xCf);		// User Stack Segment

	// IDC Why GDT_BASEADDR is 0x800, but the subject says so :|
	// memcpy((void *)GDT_BASEADDR, g_gdtEntries, (GDT_ENTRIES * sizeof(_gdtEntry)) - 1);
	g_gdtPtr.base = (uint32_t)&g_gdtEntries;
	g_gdtPtr.limit = (GDT_ENTRIES * sizeof(_gdtEntry)) - 1;

	gdtLoader(&g_gdtPtr);
}