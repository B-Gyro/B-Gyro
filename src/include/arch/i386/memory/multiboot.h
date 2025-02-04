# pragma once

# include "klibc/types.h"

# define MULTIBOOT_MAGIC_NBR	0x2BADB002

/* The symbol table for a.out. */
typedef struct aoutSymbolTable {
	uint32_t tabsize;
	uint32_t strsize;
	uint32_t addr;
	uint32_t reserved;
} _aoutSymbolTable;

/* The section header table for ELF. */
typedef struct elfSectionHeaderTable {
	uint32_t num;
	uint32_t size;
	uint32_t addr;
	uint32_t shndx;
} _elfSectionHeaderTable;

typedef struct multibootInfo {
	/* Multiboot info version number */
	uint32_t flags;

	/* Available memory from BIOS */
	uint32_t memLower;
	uint32_t memUpper;

	/* "root" partition */
	uint32_t bootDevice;

	/* Kernel command line */
	uint32_t cmdLine;

	/* Boot-Module list */
	uint32_t modsCount;
	uint32_t modsAddr;

	union {
		_aoutSymbolTable aoutSym;
		_elfSectionHeaderTable elfSec;
	} u;

	/* Memory Mapping buffer */
	uint32_t mmapLength;
	uint32_t mmapAddr;

	/* Drive Info buffer */
	uint32_t drivesLength;
	uint32_t drivesAddr;

	/* ROM configuration table */
	uint32_t configTable;

	/* Boot Loader Name */
	uint32_t bootLoaderName;

	/* APM table */
	uint32_t apmTable;

	/* Video */
	uint32_t vbeControlInfo;
	uint32_t vbe_modeInfo;
	uint16_t vbe_mode;
	uint16_t vbeInterfaceSeg;
	uint16_t vbeInterfaceOff;
	uint16_t vbeInterfaceLen;

	uint32_t frameBufferAddrLower;
	uint32_t frameBufferAddrUpper;
	uint32_t frameBufferPitch;
	uint32_t frameBufferWidth;
	uint32_t frameBufferHeight;
	uint8_t frameBufferBpp;
	#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED 0
	#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB     1
	#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT     2
	uint8_t frameBufferType;
	union {
		struct {
		uint32_t frameBufferPaletteAddr;
		uint16_t frameBufferPaletteNumColors;
		};
		struct {
		uint8_t frameBufferRedFieldPosition;
		uint8_t frameBufferRedMaskSize;
		uint8_t frameBuffer_greenFieldPosition;
		uint8_t frameBuffer_greenMaskSize;
		uint8_t frameBufferBlueFieldPosition;
		uint8_t frameBufferBlueMaskSize;
		};
	};
} _multibootInfo;
