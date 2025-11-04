#pragma once
# include "klibc/types.h"
# include "multiboot.h"

// 0x00000000 -------------------- 0xBFFFFFFF   (Lower 3 GB)
//             User space

// 0xC0000000 -------------------- 0xFFFFFFFF   (Upper 1 GB)
//             Kernel space


//             Virtual Address Space (32-bit, 3 G / 1 G split)
// ┌───────────────────────────────────────────────────────────────┐
// │ USER SPACE  (per-process)                                     │
// │                                                               │
// │ 0x00000000 ───────────────────────────────────────────────────┤
// │ |  User code / data / heap / stack / mmap / libraries       | │
// │ |  (managed by each process’s page directory)               | │
// │ |  Typically up to 3 GB total                               | │
// │                                                               │
// │ 0xBFFFFFFF ───────────────────────────────────────────────────┤
// │ Transition to kernel-only virtual addresses                   │
// ├───────────────────────────────────────────────────────────────┤
// │ KERNEL SPACE  (global, same in all processes)                 │
// │                                                               │
// │ 0xC0000000 ───────────────────────────────────────────────────┤
// │ |  Direct-mapped physical memory (“lowmem”)                 | │
// │ |  Virtual = physical + 0xC0000000                          | │
// │ |  Contains:                                                | │
// │ |   • kernel text/data/BSS                                  | │
// │ |   • page tables                                           | │
// │ |   • per-task kernel stacks (each 4–8 KB)                  | │
// │ |   • slab caches, low-mem pages                            | │
// │ |  [This region extends for size of physical lowmem]        | │
// │                                                               │
// │     Example: if 512 MB RAM → maps 0xC0000000–0xE0000000       │
// │                                                               │
// │ 0xC0800000 ───────────────────────────────────────────────────┤
// │ |  Gap / alignment buffer (≈8 MB by convention)             | │
// │ |  Prevents accidental overlap                              | │
// │                                                               │
// │ 0xC0800000 ───────────────────────────────────────────────────┤
// │ |  Kernel “heap” / vmalloc area                             | │
// │ |  Used by:                                                 | │
// │ |   • vmalloc(), vmap()                                     | │
// │ |   • ioremap() mappings below Fixmap                       | │
// │ |  Virtually contiguous, physically discontiguous pages     | │
// │ |  (≈0xC0800000–0xF7FFFFFF)                                 | │
// │                                                               │
// │ 0xF8000000 ───────────────────────────────────────────────────┤
// │ |  High-mem temporary mappings (kmap, pkmap)                | │
// │ |  IO-remap region                                          | │
// │ |  Architecture-specific mappings                           | │
// │                                                               │
// │ 0xFFF00000 ───────────────────────────────────────────────────┤
// │ |  Fixmap region                                            | │
// │ |   • APIC, BIOS, CPU local data                            | │
// │ |   • Kernel vsyscall page                                  | │
// │ |  Computed downward from 0xFFFFF000                        | │
// │                                                               │
// │ 0xFFFFFFFF ───────────────────────────────────────────────────┘



# define KERNEL_START		0xC0000000

# define KERNEL_HEAP_START	0xC1000000	// KERNEL START + 16 MB
# define KERNEL_HEAP_END	0xF8000000
# define KERNEL_PAGES_NBR	(KERNEL_HEAP_END - KERNEL_HEAP_START) / PAGE_SIZE

# define MOV_TO_HIGHER_HALF(x)	(char *)((char *)x + KERNEL_START)

typedef struct physicalMemory {
	uintptr_t	start;
	uintptr_t	end;
	uint32_t	size;
	uint32_t	maxPages;
	uint32_t	freePages;
} _physMemory;

extern _physMemory	g_physMemory;
extern char __KERNEL_VIRTUAL_END[];
extern char __KERNEL_PHYSICAL_END[];

void	initPhysicalMemory(_multibootInfo *info);
void	invalidate(uint32_t addr);

void		freeFrame(uint32_t ptr);
uint32_t	allocFrame(uint32_t end);

void	*kmmap(size_t size);
void	kunmap(uint32_t vaddr);