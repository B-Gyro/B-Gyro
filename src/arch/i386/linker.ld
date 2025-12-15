OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

ENTRY(__start)

SECTIONS
{
	. = 0x00100000;

	__KERNEL_PHYSICAL_START = .;
	
    .multiboot BLOCK(4K) : ALIGN(4K){
        *(.multiboot)
    }

    .boot BLOCK(4K) : ALIGN(4K) {
        *(.boot)
    }

	. += 0xC0000000;

	/* The kernel will live at 3GB + 1MB in the virtual
		address space, which will be mapped to 1MB in the
		physical address space. */

	__KERNEL_VIRTUAL_START = .;

	.text BLOCK(4K) : AT(ADDR(.text) - 0xC0000000) ALIGN(4K) {
		*(.text)
	}

	.rodata BLOCK(4K) : AT(ADDR(.rodata) - 0xC0000000) ALIGN(4K) {
		*(.rodata)
	}

	.data BLOCK(4K) : AT(ADDR(.data) - 0xC0000000) ALIGN(4K){
		*(.data)
	}


	.bss BLOCK(4K) : AT(ADDR(.bss) - 0xC0000000) ALIGN(4K){
		*(COMMON)
		*(.bss)
	}

	PROVIDE(__KERNEL_VIRTUAL_END = .);
	PROVIDE(__KERNEL_PHYSICAL_END = . - 0xC0000000);
}
