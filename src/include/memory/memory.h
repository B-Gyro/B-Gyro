#pragma once

// 0x00000000 -------------------- 0xBFFFFFFF   (Lower 3 GB)
//             User space

// 0xC0000000 -------------------- 0xFFFFFFFF   (Upper 1 GB)
//             Kernel space

# define KERNEL_START			0xC0000000
# define MOV_TO_HIGHER_HALF(x)	(char *)((char *)x + KERNEL_START)