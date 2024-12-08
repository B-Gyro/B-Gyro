;Set the architecture to 32-bit mode.
BITS 32

global start
extern kmain

; Multiboot Header
section	.multiboot
	ALIGN 4
	DD	0X1BADB002	; Magic number for Multiboot
	DD	0X00000000	; Flags for the bootloader (set to 0 for minimal)
	DD	-(0X1BADB002 + 0X00000000)	; Checksum to validate the magic number and flags

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
RESB 16384	 ; Reserve 16KB for the stack
stack_space: