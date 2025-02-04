OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

ENTRY(start)

SECTIONS
{
    . = 0x00100000;
	_kernel_start = .;
	
    .multiboot BLOCK(4K) : ALIGN(4K){
        *(.multiboot)
    }

    .boot BLOCK(4K) : ALIGN(4K) {
        *(.boot)
    }

    . += 0xC0000000;

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

    _kernel_end = .;

}
