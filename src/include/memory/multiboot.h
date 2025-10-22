# pragma once

# include "klibc/types.h"

# define HEADER_MAGIC_NBR		0x1BADB002
# define BOOTLOADER_MAGIC_NBR	0x2BADB002

// _multibootInfo flags
# define MULTIBOOT_MEMORY_BIT			0
# define MULTIBOOT_BOOT_DEVICE_BIT		1
# define MULTIBOOT_COMANDLINE_BIT		2
# define MULTIBOOT_MODULES_BIT			3
# define MULTIBOOT_SYMS_BIT_1			4
# define MULTIBOOT_SYMS_BIT_2			5
# define MULTIBOOT_MMAP_BIT				6
# define MULTIBOOT_DRIVES_BIT			7
# define MULTIBOOT_CONFIG_TABLE_BIT		8
# define MULTIBOOT_BOOT_LOADER_NAME_BIT	9
# define MULTIBOOT_APM_TABLE_BIT		10
# define MULTIBOOT_VBE_BIT				11
# define MULTIBOOT_VIDEO_BIT			12

// # define FRAMEBUFFER_TYPE_INDEXED	0
// # define FRAMEBUFFER_TYPE_RGB		1
// # define FRAMEBUFFER_TYPE_EGA_TEXT	2

// multiboot mmap
struct memoryMap {
	uint32_t size;
	uint64_t addr;
	uint64_t len;
	uint32_t type;
} __attribute__((packed));

typedef struct memoryMap _memoryMap;

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
	uint64_t memory;

	/* "root" partition */
	uint32_t bootDevice;

	/* Kernel command line */
	uint32_t cmdLine;

	/* Boot-Module list */
	uint32_t modsCount;
	uint32_t modsAddr;

	union {
		_aoutSymbolTable aoutSymble;
		_elfSectionHeaderTable elfSection;
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
	uint32_t vbeModeInfo;
	uint16_t vbeMode;
	uint16_t vbeInterfaceSeg;
	uint16_t vbeInterfaceOff;
	uint16_t vbeInterfaceLen;

	uint64_t frameBufferAddr;
	uint32_t frameBufferPitch;
	uint32_t frameBufferWidth;
	uint32_t frameBufferHeight;
	uint8_t frameBufferBpp;

	uint8_t frameBufferType;
	union {
		struct {
			uint32_t frameBufferPaletteAddr;
			uint16_t frameBufferPaletteNumColors;
		};
		struct {
			uint8_t frameBufferRedFieldPosition;
			uint8_t frameBufferRedMaskSize;
			uint8_t frameBufferGreenFieldPosition;
			uint8_t frameBufferGreenMaskSize;
			uint8_t frameBufferBlueFieldPosition;
			uint8_t frameBufferBlueMaskSize;
		};
	};
} _multibootInfo;
