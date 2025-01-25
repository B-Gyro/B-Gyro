;Set the architecture to 32-bit mode.
BITS 32

global start
extern kmain

MAGICNBR	equ	0x1BADB002
MBFLAGS		equ	0
CHECKSUM	equ	-(MAGICNBR + MBFLAGS)

; Multiboot Header
section	.multiboot
	ALIGN 4
	DD	MAGICNBR	; Magic number for Multiboot
	DD	MBFLAGS		; Flags for the bootloader
	DD	CHECKSUM	; Checksum to validate the magic number and flags

; Kernel Entry Point
start:
	CLI						; Disable interrupts (initially)
	MOV esp, stack_space	; Set up the stack by moving the stack pointer to `stack_space`
	CALL kmain				; Call the kernel's main function
	HLT						; Halt execution (for safety)

Halt:
	CLI
	HLT
	JMP Halt

; Stack Allocation in BSS Section
section .bss
RESB 1048576	 ; Reserve 1MB for the stack
stack_space: