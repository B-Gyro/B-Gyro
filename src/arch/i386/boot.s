BITS 32

STACK_SIZE	equ	104856   ; 16 MiB

PAGE_ALIGN	equ	1 << 0	; align loaded modules on page boundaries
MEM_INFO	equ	1 << 1	; provide memory map
USE_GFX		equ	0

MAGICNBR	equ	0x1BADB002						; 'magic number' lets bootloader find the header
FLAGS		equ PAGE_ALIGN | MEM_INFO | USE_GFX
CHECKSUM	equ	-(MAGICNBR + FLAGS)				; checksum of above, to prove we are multiboot

SECTION .multiboot
ALIGN 4
    DD MAGICNBR
    DD FLAGS
    DD CHECKSUM

; Allocate the initial stack
SECTION .bss
ALIGN 16
stack:
	RESB STACK_SIZE

; The kernel entry point
SECTION .boot
global start
start:
	; set up the page directoryy
    MOV	ecx, (bootPageDirectory - 0xC0000000)
    MOV	cr3, ecx

	; it enables 4MB pages instead of the default 4KB
    MOV	ecx, cr4
    OR	ecx, 0x10
    MOV	cr4, ecx

	; enable paging and the write-protect bit
    MOV	ecx, cr0
    OR	ecx, 0x80010000
    MOV	cr0, ecx

	; eump to higher half
    JMP	higherHalf

SECTION .text
extern	kmain
higherHalf:
    MOV		esp, stack + STACK_SIZE
    PUSH	ebx
    PUSH	eax
    XOR		ebp, ebp
	; call halt
    CALL	kmain

halt:
    HLT
    JMP halt


SECTION .data
align 4096
global bootPageDirectory
bootPageDirectory:
    DD 10000011b
    TIMES 768-1 DD 0

    DD (0 << 22) | 0b10000011
    DD (1 << 22) | 0b10000011
    DD (2 << 22) | 0b10000011
    DD (3 << 22) | 0b10000011
    TIMES 256-4 DD 0