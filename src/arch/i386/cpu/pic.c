# include "arch/i386/cpu/descriptorTables.h"
# include "arch/i386/ports/portsIO.h"

/*
	we need to remap the IRQs of the pic slave and master
	before remapping:
		IRQs 0-7  ->  IDT_ENTRYs    8-15
		IRQs 8-15 ->  IDT_ENTRYs    0x70-0x78
	we want to map it to:
		IRQs 0-15 ->  IDT_ENTRYs    32-47
*/

void	picRemap(uint8_t masterOffset, uint8_t slaveOffset){
	uint8_t a1, a2;
 
	a1 = portByteIn(MASTER_DATA);				// save masks
	a2 = portByteIn(SLAVE_DATA);
 
	// starts the initialization sequence (in cascade mode)
	portByteOut(MASTER_CTRL, 0x10 | 0x01);
	ioWait();
	portByteOut(SLAVE_CTRL, 0x10 | 0x01);
	ioWait();
	// ICW2: Master PIC vector offset
	portByteOut(MASTER_DATA, masterOffset);
	ioWait();
	// ICW2: Slave PIC vector offset
	portByteOut(SLAVE_DATA, slaveOffset);
	ioWait();
	// ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	portByteOut(MASTER_DATA, 4);
	ioWait();
	// ICW3: tell Slave PIC its cascade identity (0000 0010)
	portByteOut(SLAVE_DATA, 2);
	ioWait();
	// ICW4: have the PICs use 8086 mode (and not 8080 mode)
	portByteOut(MASTER_DATA, 0x01);
	ioWait();
	portByteOut(SLAVE_DATA, 0x01);
	ioWait();
	// restore saved masks.
	portByteOut(MASTER_DATA, a1);
	portByteOut(SLAVE_DATA, a2);
}