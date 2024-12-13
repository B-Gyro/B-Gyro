# include "klibc/print.h"
# include "arch/i386/ports/portsIO.h"
# include "arch/i386/cpu/descriptorTables.h"


void *g_IRQRoutines[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

// set a handler to the IRQ
void	setIRQHandler(uint8_t irq, void (*handler)(_registers r)){
	g_IRQRoutines[irq] = handler;
	SERIAL_INFO("IRQ Handler set for IRQ %d", irq);
}
// reset the handler to NULL
void	clearIRQHandler(uint8_t irq){
	g_IRQRoutines[irq] = 0;
	SERIAL_INFO("IRQ %d handler cleared", irq);
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
	uint8_t irq;

	irq = Rs.InterruptNbr - 32;
	if (irq < 16 && g_IRQRoutines[irq])
		f = (void *)g_IRQRoutines[irq], f(Rs);
	else
		SERIAL_ERR("IRQ %d not handled\n", irq);
	picEOI(irq);
}