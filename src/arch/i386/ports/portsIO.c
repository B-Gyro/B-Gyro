#include "klibc/types.h"

// asm ( assembler template : output operands : input operands);

// Read a Byte from portNumber
uint8_t portByteIn(uint16_t portNumber){
	uint8_t result;
	/*
		reading from a port in assembly will be:
			mov dx, 0x03C4    ; set DX to the port number
			in al, dx        ; reading from port 0x03C4 and store in al
		so we do the same here except GNU uses AT&T syntax, which make the order of the operands reversed.
		also we are saying to store the value of al into result. and the the port number into dx
	*/
	__asm__ __volatile__ ("in %%dx, %%al" : "=a"(result) : "d"(portNumber));
	return result;
}

// Read a Word from portNumber
uint16_t portWordIn(uint16_t portNumber){
	uint16_t result;
	__asm__ __volatile__ ("in %%dx, %%ax" : "=a"(result) : "d"(portNumber));
	return result;
}

// Write a Byte to portNumber
void	portByteOut(uint16_t portNumber, uint8_t value){
	/*
		writing to a port in assembly will be:
			mov dx, 0x03C4    ; set DX to the port number
			mov al, 0x45    ; set al with the value to print
			out dx, al        ; write to port 0x03C4 the value 0x45
	*/
	__asm__ __volatile__ ("out %%al, %%dx" : : "a"(value), "d"(portNumber));
}

// Write a Word to portNumber
void	portWordOut(uint16_t portNumber, uint16_t value){
	__asm__ __volatile__ ("out %%ax, %%dx" : : "a"(value), "d"(portNumber));
}


void	portDWordOut(uint16_t portNumber, uint32_t value) {
    __asm__ __volatile__ ("outl %0, %1" : : "a"(value), "d"(portNumber));
}

uint32_t	portDWordIn(uint16_t portNumber) {
	uint32_t result;
	
	__asm__ __volatile__ ("inl %1, %0" : "=a"(result) : "d"(portNumber));
	return result;
}


/* Wait a very small amount of time (1 to 4 microseconds, generally).
Useful for implementing a small delay for PIC remapping on old hardware.*/
void ioWait(void){
    portByteOut(0x80, 0);
}