#pragma once

# define PAGE_SIZE	4096

# define ALIGN_UP(n)	(n + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
# define ALIGN_DOWN(n)	(n & ~4095)