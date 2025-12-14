# include "klibc/print.h"
# include "arch/i386/cpu/descriptorTables.h"

char *g_ISRDescriptions[] = {
    "Division by zero",                // 0
    "Debug",                           // 1
    "Non maskable interrupt",          // 2
    "Breakpoint",                      // 3
    "Into detected overflow",          // 4
    "Out of bounds",                   // 5
    "Invalid opcode",                  // 6
    "No coprocessor",                  // 7
    "Double fault",                    // 8
    "Coprocessor segment overrun",     // 9
    "Invalid TSS",                     // 10
    "Segment not present",             // 11
    "Stack-segment fault",             // 12
    "General protection fault",        // 13
    "Page fault",                      // 14
    "Reserved",                        // 15  => double free
    "Floating point exception",        // 16
    "Alignment check",                 // 17
    "Machine check",                   // 18
    "SIMD floating-point exception",   // 19
    "Virtualization exception",        // 20
    "Control protection exception",    // 21
    "Reserved",                        // 22 => out of bound 
    "Reserved",                        // 23 => free 3awra
    "Reserved",                        // 24
    "Reserved",                        // 25
    "Reserved",                        // 26
    "Reserved",                        // 27
    "Hypervisor injection exception",  // 28
    "VMM communication exception",     // 29
    "Security exception",              // 30
    "Reserved"                         // 31
};

void	isrHandler(_registers Rs){
	if (Rs.InterruptNbr < 32){
		VGA_PRINT("Exception Raised => %s, ERR_CODE: %d\r\n", g_ISRDescriptions[Rs.InterruptNbr], Rs.errCode);
		SERIAL_ERR("Exception Raised => %s, ERR_CODE: %d\r\n", g_ISRDescriptions[Rs.InterruptNbr], Rs.errCode);
		__asm__ __volatile__ ("cli; hlt");
	}
}