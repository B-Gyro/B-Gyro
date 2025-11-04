BITS 32
global _start

;****************************************************************************************************;
;*											  MULTIBOOT  											*;
;****************************************************************************************************;
PAGE_ALIGN	equ	1 << 0	; ALIGN loaded modules on page boundaries
MEM_INFO	equ	1 << 1	; provide memory map

FLAGS		equ PAGE_ALIGN | MEM_INFO
MAGICNBR	equ	0x1BADB002			; 'magic number' lets bootloader find the header
CHECKSUM	equ	-(MAGICNBR + FLAGS)	; checksum of above, to prove we are multiboot

SECTION .multiboot
ALIGN 4
	DD MAGICNBR
	DD FLAGS
	DD CHECKSUM

;****************************************************************************************************;
;*												 BSS	 											*;
;****************************************************************************************************;
global __stack_bottom
global __stack_top

STACK_SIZE	equ	16384

; Allocate the initial stack
SECTION .bss
ALIGN 4096
__stack_bottom:
	RESB STACK_SIZE
__stack_top:

;****************************************************************************************************;
;*												BOOT												*;
;****************************************************************************************************;
KERNEL_START equ 0xC0000000

; CR0
WP_BIT	equ	(1 << 16)	; write protect bit
PG_BIT	equ	(1 << 31)	; paging bit

;CR4
PSE_BIT	equ	(1 << 4)	; Page Size Extension

; The kernel entry point
SECTION .boot
extern	kmain
global __start

__start:
	cli

	; load page directory physical address into CR3
	MOV	ecx, (__BOOT_PAGE_DIRECTORY - KERNEL_START)
	MOV	cr3, ecx

	; it enables 4MB pages instead of the default 4KB
	MOV	ecx, cr4
	OR	ecx, PSE_BIT
	MOV	cr4, ecx

	; enable paging and the write-protect bit
	MOV	ecx, cr0
	OR	ecx, (PG_BIT | WP_BIT)
	MOV	cr0, ecx

	JMP higherHalf

;****************************************************************************************************;
;*												TEXT												*;
;****************************************************************************************************;
SECTION .text
extern	kmain

PAGE_PRESENT_BIT	equ (1 << 0) ; bit 0: 
PAGE_WRITE_BIT		equ (1 << 1) ; bit 1: 

higherHalf:
	MOV		esp, __stack_top	; stack_bottom + STACK_SIZE

	; push	esp ; stack
	PUSH	ebx ; multiboot data
	PUSH	eax	; magic number
	XOR		ebp, ebp
	CALL	kmain
	CLI

halt:
	HLT
	JMP halt
	
;****************************************************************************************************;
;*												DATA												*;
;****************************************************************************************************;
P_BIT	equ (1 << 0) ; bit 0: P  The kernel page is present.
RW_BIT	equ (1 << 1) ; bit 1: RW The kernel page is read/write.
PS_BIT	equ (1 << 7) ; bit 7: PS The kernel page is 4MB.

KERNEL_PAGE_NUMBER equ (0xC0000000 >> 22)	; 0xC0000000 / 4096KB == 0xC0000000 / 4MB

section .data
align 4096
global __BOOT_PAGE_DIRECTORY
__BOOT_PAGE_DIRECTORY:
	;The multiboot header is very small:
	;32 bytes minimum (magic, flags, checksum, some optional fields)
	;Some optional info like framebuffer info adds a few more bytes (~32–64 B).
	;It must be located within the first 8 KB of the kernel binary so GRUB can find it.
	;So 1 PDE is enough
	DD 0 | (P_BIT | RW_BIT | PS_BIT)
	
	TIMES (KERNEL_PAGE_NUMBER - 1) DD 0			; Pages before kernel space.
	
	; On x86, Linux typically maps at least the first 8–16 MB of physical memory 
	; with 4 MB PDEs (or multiple 4 KB pages in some configurations) 
	; so it can safely access kernel code, data, and initial stack.
	; This page directory entry defines a 16MB (4MB x 4) page containing the kernel.
	DD (0 << 22) | (P_BIT | RW_BIT | PS_BIT)
	DD (1 << 22) | (P_BIT | RW_BIT | PS_BIT)
	DD (2 << 22) | (P_BIT | RW_BIT | PS_BIT)
	DD (3 << 22) | (P_BIT | RW_BIT | PS_BIT)
	TIMES (1024 - KERNEL_PAGE_NUMBER - 4) DD 0	; Pages after the kernel image.
