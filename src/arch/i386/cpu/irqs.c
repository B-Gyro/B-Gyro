# include "klibc/print.h"
# include "arch/i386/ports/portsIO.h"
# include "arch/i386/cpu/descriptorTables.h"


void *IRQRoutines[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

// set a handler to the IRQ
void	setIRQHandler(uint8_t irq, void (*handler)(_registers r)){
	IRQRoutines[irq] = handler;
	SERIAL_INFO("IRQ Handler set for IRQ %d\n", irq);
}
// reset the handler to NULL
void	clearIRQHandler(uint8_t irq){
	IRQRoutines[irq] = 0;
	SERIAL_INFO("IRQ %d handler cleared\n", irq);
}
// send End of Interrupt to the Pics
void	picEOI(uint8_t irq){
	if (irq >= 8)
		portByteOut(SLAVE_CTRL, PIC_EOI);
	portByteOut(MASTER_CTRL, PIC_EOI);
}

// IRQ handler
void	irqHandler(_registers Rs){
	void (*f)(_registers);
	uint8_t irq = Rs.intNbr - 32;

	if (irq < 16 && IRQRoutines[irq])
		f = (void *)IRQRoutines[irq], f(Rs);
	else
		SERIAL_ERR("IRQ %d not handled\n", irq);
	picEOI(irq);
}