#include "klibc/types.h"
#include "images/image.h"

// to do: change pixels from char *[] to int *[] | store COLOR instead of CHAR

// # define E -1
// # define B BODER_COLOR
// # define F FILL_COLOR
// # define S SHADOW_COLOR

// _image defaultCursorImage = { 
// 	.width = 14, 
// 	.height = 22,
// 	.pixels = {
// 		{E,B,E,E,E,E,E,E,E,E,E,E,E,E},
// 		{S,B,B,E,E,E,E,E,E,E,E,E,E,E},
// 		{S,B,F,B,E,E,E,E,E,E,E,E,E,E},
// 		{S,B,F,F,B,E,E,E,E,E,E,E,E,E},
// 		{S,B,F,F,F,B,E,E,E,E,E,E,E,E},
// 		{S,B,F,F,F,F,B,E,E,E,E,E,E,E},
// 		{S,B,F,F,F,F,F,B,E,E,E,E,E,E},
// 		{S,B,F,F,F,F,F,F,B,E,E,E,E,E},
// 		{S,B,F,F,F,F,F,F,F,B,E,E,E,E},
// 		{S,B,F,F,F,F,F,F,F,F,B,E,E,E},
// 		{S,B,F,F,F,F,F,F,F,F,F,B,E,E},
// 		{S,B,F,F,F,F,F,F,F,F,F,F,B,E},
// 		{S,B,F,F,F,F,F,F,F,F,F,F,F,B},
// 		{S,B,F,F,F,F,F,F,B,B,B,B,B,B},
// 		{S,B,F,F,F,B,F,F,B,S,S,S,S,S},
// 		{S,B,F,F,B,S,B,F,F,B,E,E,E,E},
// 		{S,B,F,B,S,S,B,F,F,B,E,E,E,E},
// 		{S,B,B,S,E,E,S,B,F,F,B,E,E,E},
// 		{S,B,S,S,E,E,S,B,F,F,B,E,E,E},
// 		{S,S,S,E,E,E,E,S,B,F,B,E,E,E},
// 		{E,E,E,E,E,E,E,E,S,B,E,E,E,E},
// 		{E,E,E,E,E,E,E,E,S,S,E,E,E,E},
// 	}
// };

_image defaultCursorImage = { 
	.width = 14, 
	.height = 22,
	.pixels = {
		" B            ",
		"SBB           ",
		"SBFB          ",
		"SBFFB         ",
		"SBFFFB        ",
		"SBFFFFB       ",
		"SBFFFFFB      ",
		"SBFFFFFFB     ",
		"SBFFFFFFFB    ",
		"SBFFFFFFFFB   ",
		"SBFFFFFFFFFB  ",
		"SBFFFFFFFFFFB ",
		"SBFFFFFFFFFFFB",
		"SBFFFFFFBBBBBB",
		"SBFFFBFFBSSSS ",
		"SBFFBSBFFB    ",
		"SBFBSSBFFB    ",
		"SBBS  SBFFB   ",
		"SBSS  SBFFB   ",
		"SSS    SBFB   ",
		"        SB    ",
		"        SS    "
	}
};

_image selectCursorImage = { 
	.width = 7, 
	.height = 18,
	.pixels = {
		"BBB-BBB",
		"BFFBFFB",
		"BBBFBBB",
		"  BFB  ",
		"  BFB  ",
		"  BFB  ",
		"  BFB  ",
		"  BFB  ",
		"  BFB  ",
		"  BFB  ",
		"  BFB  ",
		"  BFB  ",
		"  BFB  ",
		"  BFB  ",
		"  BFB  ",
		"BBBFBBB",
		"BFFBFFB",
		"BBB-BBB",
	}
};


_image waitingCursorImage = { 
	.width = 15, 
	.height = 27,
	.pixels = {
		"BBBBBBBBBBBBBBB",
		"BBBBBBBBBBBBBBB",
		"BBFFFFFFFFFFFBB",
		" BBBBBBBBBBBBB ",
		" BBFFFFFFFFFBB ",
		" BBFFFFFFFFFBB ",
		" BBFFBFFFBFFBB ",
		" BBFBFBFBFBFBB ",
		" BBFFBFBFBFFBB ",
		"  BBFFBFBFFBB  ",
		"   BBFFBFFBB   ",
		"    BBFBFBB    ",
		"     BBFBB     ",
		"     BBFBB     ",
		"     BBFBB     ",
		"    BBFBFBB    ",
		"   BBFFBFFBB   ",
		"  BBFFBFBFFBB  ",
		" BBFFBFBFBFFBB ",
		" BBFBFBFBFBFBB ",
		" BBFFBFFFBFFBB ",
		" BBFFFFFFFFFBB ",
		" BBFFFFFFFFFBB ",
		" BBBBBBBBBBBBB ",
		"BBFFFFFFFFFFFBB",
		"BBBBBBBBBBBBBBB",
		"BBBBBBBBBBBBBBB",
	}
};

_image moveCursorImage = { 
	.width = 27, 
	.height = 27,
	.pixels = {
		"             B             ",
		"            BFB            ",
		"           BBFBB           ",
		"          BBFFFBB          ",
		"         BBFFFFFBB         ",
		"        BBFFFFFFFBB        ",
		"        BFFFFFFFFFB        ",
		"        BBBBFFFBBBB        ",
		"     BBB   BFFFB   BBB     ",
		"    BBFB   BFFFB   BFBB    ",
		"   BBFFB   BFFFB   BFFBB   ",
		"  BBFFFBBBBBFFFBBBBBFFFBB  ",
		" BBFFFFFFFFFFFFFFFFFFFFFBB ",
		"BFFFFFFFFFFFFFFFFFFFFFFFFFB",
		" BBFFFFFFFFFFFFFFFFFFFFFBB ",
		"  BBFFFBBBBBFFFBBBBBFFFBB  ",
		"   BBFFB   BFFFB   BFFBB   ",
		"    BBFB   BFFFB   BFBB    ",
		"     BBB   BFFFB   BBB     ",
		"        BBBBFFFBBBB        ",
		"        BFFFFFFFFFB        ",
		"        BBFFFFFFFBB        ",
		"         BBFFFFFBB         ",
		"          BBFFFBB          ",
		"           BBFBB           ",
		"            BFB            ",
		"             B             ",
	}
};


_image resizeHeightCursorImage = { 
	.width = 13, 
	.height = 23,
	.pixels = {
		"      B      ",
		"     BFB     ",
		"    BBFBB    ",
		"   BBFFFBB   ",
		"  BBFFFFFBB  ",
		" BBFFFFFFFBB ",
		"BBFFFFFFFFFBB",
		"BBBBBFFFBBBBB",
		"    BFFFB    ",
		"    BFFFB    ",
		"    BFFFB    ",
		"    BFFFB    ",
		"    BFFFB    ",
		"    BFFFB    ",
		"    BFFFB    ",
		"BBBBBFFFBBBBB",
		"BBFFFFFFFFFBB",
		" BBFFFFFFFBB ",
		"  BBFFFFFBB  ",
		"   BBFFFBB   ",
		"    BBFBB    ",
		"     BFB     ",
		"      B      ",
	}
};


_image resizeWidthCursorImage = { 
	.width = 23, 
	.height = 13,
	.pixels = {
		"      BB       BB      ",
		"     BBB       BBB     ",
		"    BBFB       BFBB    ",
		"   BBFFB       BFFBB   ",
		"  BBFFFBBBBBBBBBFFFBB  ",
		" BBFFFFFFFFFFFFFFFFFBB ",
		"BFFFFFFFFFFFFFFFFFFFFFB",
		" BBFFFFFFFFFFFFFFFFFBB ",
		"  BBFFFBBBBBBBBBFFFBB  ",
		"   BBFFB       BFFBB   ",
		"    BBFB       BFBB    ",
		"     BBB       BBB     ",
		"      BB       BB      ",
	}
};

_image resizeCorner1Image = { 
	.width = 12, 
	.height = 12,
	.pixels = {
		"    BBBBBBBB",
		"    BFFFFFFB",
		"    BBFFFFFB",
		"     BBFFFFB",
		"BBB BBFFFFFB",
		"BFBBBFFFBFFB",
		"BFFBFFFBBBFB",
		"BFFFFFBB BBB",
		"BFFFFBB     ",
		"BFFFFFBB    ",
		"BFFFFFFB    ",
		"BBBBBBBB    ",
	}
};


_image resizeCorner2Image = { 
	.width = 12, 
	.height = 12,
	.pixels = {
		"BBBBBBBB    ",
		"BFFFFFFB    ",
		"BFFFFFBB    ",
		"BFFFFBB     ",
		"BFFFFFBB BBB",
		"BFFBFFFBBBFB",
		"BFBBBFFFBFFB",
		"BBB BBFFFFFB",
		"     BBFFFFB",
		"    BBFFFFFB",
		"    BFFFFFFB",
		"    BBBBBBBB",

	}
};


// _image textCursorImage = { 
// 	.width = 8, 
// 	.height = 16,
// 	.pixels = {
// 		"BBBBBBBB",
// 		"BBBBBBBB",
// 		"BBBBBBBB",
// 		"BBBBBBBB",
// 		"BBBBBBBB",
// 		"BBBBBBBB",
// 		"BBBBBBBB",
// 		"BBBBBBBB",
// 		"BBBBBBBB",
// 		"BBBBBBBB",
// 		"BBBBBBBB",
// 		"BBBBBBBB",
// 		"BBBBBBBB",
// 		"BBBBBBBB",
// 		"BBBBBBBB",
// 		"BBBBBBBB",
// 	}
// };

// _image textCursorImage = { 
// 	.width = 8, 
// 	.height = 16,
// 	.pixels = {
// 		"        ",
// 		"        ",
// 		"        ",
// 		"        ",
// 		"        ",
// 		"        ",
// 		"        ",
// 		"        ",
// 		"        ",
// 		"        ",
// 		"        ",
// 		"        ",
// 		"        ",
// 		"        ",
// 		"        ",
// 		"BBBBBBBB",
// 	}
// };