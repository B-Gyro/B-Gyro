# pragma once

# include "klibc/types.h"

# define MEMORY_AVAILABLE			1
# define MEMORY_RESERVED			2
# define MEMORY_ACPI_RECLAIMABLE	3
# define MEMORYNVS					4
# define MEMORYBADRAM				5

extern uint32_t	bootPageDirectory[1024];

struct memoryMap {
	uint32_t size;
	uint32_t addrLower;
	uint32_t addrUpper;
	uint32_t lenLower;
	uint32_t lenUpper;

	uint32_t type;
} __attribute__((packed));
typedef struct memoryMap _memoryMap;