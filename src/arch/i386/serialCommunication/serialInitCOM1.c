#include "arch/i386/ports/portsIO.h"
#include "arch/i386/ports/serialCommunication/serialIO.h"

// init the serial communication to use the UART Protocol
int initSerial() {
	// Disable all interrupts (SERIAL Interupts), making it so you need  to check if there is an input
	portByteOut(COM1 + 1, 0x00);
	// Enable DLAB (set baud rate divisor), enables you to change the rate
	portByteOut(COM1 + 3, 0x80);
	portByteOut(COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	portByteOut(COM1 + 1, 0x00);    //                  (hi byte)
	portByteOut(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
	portByteOut(COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	portByteOut(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
	portByteOut(COM1 + 4, 0x1E);    // Set in loopback mode, test the serial chip
	portByteOut(COM1 + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

	// Check if serial is faulty (i.e: not same byte as sent)
	if(portByteIn(COM1 + 0) != 0xAE)
      return 1;
 
	// If serial is not faulty set it in normal operation mode
	// (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
	portByteOut(COM1 + 4, 0x0F);
	return 0;
}
