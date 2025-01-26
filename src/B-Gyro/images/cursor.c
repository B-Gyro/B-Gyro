#include "klibc/types.h"
#include "images/image.h"

# define E -1
# define B BODER_COLOR
# define F FILL_COLOR
# define S SHADOW_COLOR

_image img_defaultCursor = { 
	.width = 14, 
	.height = 22,
	.pixels = {
		{E,B,E,E,E,E,E,E,E,E,E,E,E,E},
		{S,B,B,E,E,E,E,E,E,E,E,E,E,E},
		{S,B,F,B,E,E,E,E,E,E,E,E,E,E},
		{S,B,F,F,B,E,E,E,E,E,E,E,E,E},
		{S,B,F,F,F,B,E,E,E,E,E,E,E,E},
		{S,B,F,F,F,F,B,E,E,E,E,E,E,E},
		{S,B,F,F,F,F,F,B,E,E,E,E,E,E},
		{S,B,F,F,F,F,F,F,B,E,E,E,E,E},
		{S,B,F,F,F,F,F,F,F,B,E,E,E,E},
		{S,B,F,F,F,F,F,F,F,F,B,E,E,E},
		{S,B,F,F,F,F,F,F,F,F,F,B,E,E},
		{S,B,F,F,F,F,F,F,F,F,F,F,B,E},
		{S,B,F,F,F,F,F,F,F,F,F,F,F,B},
		{S,B,F,F,F,F,F,F,B,B,B,B,B,B},
		{S,B,F,F,F,B,F,F,B,S,S,S,S,S},
		{S,B,F,F,B,S,B,F,F,B,E,E,E,E},
		{S,B,F,B,S,S,B,F,F,B,E,E,E,E},
		{S,B,B,S,E,E,S,B,F,F,B,E,E,E},
		{S,B,S,S,E,E,S,B,F,F,B,E,E,E},
		{S,S,S,E,E,E,E,S,B,F,B,E,E,E},
		{E,E,E,E,E,E,E,E,S,B,E,E,E,E},
		{E,E,E,E,E,E,E,E,S,S,E,E,E,E},
	}
};

_image img_selectCursor = { 
	.width = 7, 
	.height = 18,
	.pixels = {
		{B,B,B,E,B,B,B},
		{B,F,F,B,F,F,B},
		{B,B,B,F,B,B,B},
		{E,E,B,F,B,E,E},
		{E,E,B,F,B,E,E},
		{E,E,B,F,B,E,E},
		{E,E,B,F,B,E,E},
		{E,E,B,F,B,E,E},
		{E,E,B,F,B,E,E},
		{E,E,B,F,B,E,E},
		{E,E,B,F,B,E,E},
		{E,E,B,F,B,E,E},
		{E,E,B,F,B,E,E},
		{E,E,B,F,B,E,E},
		{E,E,B,F,B,E,E},
		{B,B,B,F,B,B,B},
		{B,F,F,B,F,F,B},
		{B,B,B,E,B,B,B},
	}
};


_image img_waitingCursor = { 
	.width = 15, 
	.height = 27,
	.pixels = {
		{B,B,B,B,B,B,B,B,B,B,B,B,B,B,B},
		{B,B,B,B,B,B,B,B,B,B,B,B,B,B,B},
		{B,B,F,F,F,F,F,F,F,F,F,F,F,B,B},
		{E,B,B,B,B,B,B,B,B,B,B,B,B,B,E},
		{E,B,B,F,F,F,F,F,F,F,F,F,B,B,E},
		{E,B,B,F,F,F,F,F,F,F,F,F,B,B,E},
		{E,B,B,F,F,B,F,F,F,B,F,F,B,B,E},
		{E,B,B,F,B,F,B,F,B,F,B,F,B,B,E},
		{E,B,B,F,F,B,F,B,F,B,F,F,B,B,E},
		{E,E,B,B,F,F,B,F,B,F,F,B,B,E,E},
		{E,E,E,B,B,F,F,B,F,F,B,B,E,E,E},
		{E,E,E,E,B,B,F,B,F,B,B,E,E,E,E},
		{E,E,E,E,E,B,B,F,B,B,E,E,E,E,E},
		{E,E,E,E,E,B,B,F,B,B,E,E,E,E,E},
		{E,E,E,E,E,B,B,F,B,B,E,E,E,E,E},
		{E,E,E,E,B,B,F,B,F,B,B,E,E,E,E},
		{E,E,E,B,B,F,F,B,F,F,B,B,E,E,E},
		{E,E,B,B,F,F,B,F,B,F,F,B,B,E,E},
		{E,B,B,F,F,B,F,B,F,B,F,F,B,B,E},
		{E,B,B,F,B,F,B,F,B,F,B,F,B,B,E},
		{E,B,B,F,F,B,F,F,F,B,F,F,B,B,E},
		{E,B,B,F,F,F,F,F,F,F,F,F,B,B,E},
		{E,B,B,F,F,F,F,F,F,F,F,F,B,B,E},
		{E,B,B,B,B,B,B,B,B,B,B,B,B,B,E},
		{B,B,F,F,F,F,F,F,F,F,F,F,F,B,B},
		{B,B,B,B,B,B,B,B,B,B,B,B,B,B,B},
		{B,B,B,B,B,B,B,B,B,B,B,B,B,B,B},
	}
};

_image img_moveCursor = { 
	.width = 27, 
	.height = 27,
	.pixels = {
		{E,E,E,E,E,E,E,E,E,E,E,E,E,B,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,B,F,B,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,B,B,F,B,B,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,B,B,F,F,F,B,B,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,B,B,F,F,F,F,F,B,B,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,B,B,F,F,F,F,F,F,F,B,B,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,B,F,F,F,F,F,F,F,F,F,B,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,B,B,B,B,F,F,F,B,B,B,B,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,B,B,B,E,E,E,B,F,F,F,B,E,E,E,B,B,B,E,E,E,E,E},
		{E,E,E,E,B,B,F,B,E,E,E,B,F,F,F,B,E,E,E,B,F,B,B,E,E,E,E},
		{E,E,E,B,B,F,F,B,E,E,E,B,F,F,F,B,E,E,E,B,F,F,B,B,E,E,E},
		{E,E,B,B,F,F,F,B,B,B,B,B,F,F,F,B,B,B,B,B,F,F,F,B,B,E,E},
		{E,B,B,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,B,B,E},
		{B,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,B},
		{E,B,B,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,B,B,E},
		{E,E,B,B,F,F,F,B,B,B,B,B,F,F,F,B,B,B,B,B,F,F,F,B,B,E,E},
		{E,E,E,B,B,F,F,B,E,E,E,B,F,F,F,B,E,E,E,B,F,F,B,B,E,E,E},
		{E,E,E,E,B,B,F,B,E,E,E,B,F,F,F,B,E,E,E,B,F,B,B,E,E,E,E},
		{E,E,E,E,E,B,B,B,E,E,E,B,F,F,F,B,E,E,E,B,B,B,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,B,B,B,B,F,F,F,B,B,B,B,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,B,F,F,F,F,F,F,F,F,F,B,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,B,B,F,F,F,F,F,F,F,B,B,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,B,B,F,F,F,F,F,B,B,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,B,B,F,F,F,B,B,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,B,B,F,B,B,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,B,F,B,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,B,E,E,E,E,E,E,E,E,E,E,E,E,E},
	}
};


_image img_resizeHeightCursor = { 
	.width = 13, 
	.height = 23,
	.pixels = {
		{E,E,E,E,E,E,B,E,E,E,E,E,E},
		{E,E,E,E,E,B,F,B,E,E,E,E,E},
		{E,E,E,E,B,B,F,B,B,E,E,E,E},
		{E,E,E,B,B,F,F,F,B,B,E,E,E},
		{E,E,B,B,F,F,F,F,F,B,B,E,E},
		{E,B,B,F,F,F,F,F,F,F,B,B,E},
		{B,B,F,F,F,F,F,F,F,F,F,B,B},
		{B,B,B,B,B,F,F,F,B,B,B,B,B},
		{E,E,E,E,B,F,F,F,B,E,E,E,E},
		{E,E,E,E,B,F,F,F,B,E,E,E,E},
		{E,E,E,E,B,F,F,F,B,E,E,E,E},
		{E,E,E,E,B,F,F,F,B,E,E,E,E},
		{E,E,E,E,B,F,F,F,B,E,E,E,E},
		{E,E,E,E,B,F,F,F,B,E,E,E,E},
		{E,E,E,E,B,F,F,F,B,E,E,E,E},
		{B,B,B,B,B,F,F,F,B,B,B,B,B},
		{B,B,F,F,F,F,F,F,F,F,F,B,B},
		{E,B,B,F,F,F,F,F,F,F,B,B,E},
		{E,E,B,B,F,F,F,F,F,B,B,E,E},
		{E,E,E,B,B,F,F,F,B,B,E,E,E},
		{E,E,E,E,B,B,F,B,B,E,E,E,E},
		{E,E,E,E,E,B,F,B,E,E,E,E,E},
		{E,E,E,E,E,E,B,E,E,E,E,E,E},
	}
};


_image img_resizeWidthCursor = { 
	.width = 23, 
	.height = 13,
	.pixels = {
		{E,E,E,E,E,E,B,B,E,E,E,E,E,E,E,B,B,E,E,E,E,E,E},
		{E,E,E,E,E,B,B,B,E,E,E,E,E,E,E,B,B,B,E,E,E,E,E},
		{E,E,E,E,B,B,F,B,E,E,E,E,E,E,E,B,F,B,B,E,E,E,E},
		{E,E,E,B,B,F,F,B,E,E,E,E,E,E,E,B,F,F,B,B,E,E,E},
		{E,E,B,B,F,F,F,B,B,B,B,B,B,B,B,B,F,F,F,B,B,E,E},
		{E,B,B,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,B,B,E},
		{B,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,B},
		{E,B,B,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,B,B,E},
		{E,E,B,B,F,F,F,B,B,B,B,B,B,B,B,B,F,F,F,B,B,E,E},
		{E,E,E,B,B,F,F,B,E,E,E,E,E,E,E,B,F,F,B,B,E,E,E},
		{E,E,E,E,B,B,F,B,E,E,E,E,E,E,E,B,F,B,B,E,E,E,E},
		{E,E,E,E,E,B,B,B,E,E,E,E,E,E,E,B,B,B,E,E,E,E,E},
		{E,E,E,E,E,E,B,B,E,E,E,E,E,E,E,B,B,E,E,E,E,E,E},
	}
};

_image img_resizeCorner1 = { 
	.width = 12, 
	.height = 12,
	.pixels = {
		{E,E,E,E,B,B,B,B,B,B,B,B},
		{E,E,E,E,B,F,F,F,F,F,F,B},
		{E,E,E,E,B,B,F,F,F,F,F,B},
		{E,E,E,E,E,B,B,F,F,F,F,B},
		{B,B,B,E,B,B,F,F,F,F,F,B},
		{B,F,B,B,B,F,F,F,B,F,F,B},
		{B,F,F,B,F,F,F,B,B,B,F,B},
		{B,F,F,F,F,F,B,B,E,B,B,B},
		{B,F,F,F,F,B,B,E,E,E,E,E},
		{B,F,F,F,F,F,B,B,E,E,E,E},
		{B,F,F,F,F,F,F,B,E,E,E,E},
		{B,B,B,B,B,B,B,B,E,E,E,E},
	}
};


_image img_resizeCorner2 = { 
	.width = 12, 
	.height = 12,
	.pixels = {
		{B,B,B,B,B,B,B,B,E,E,E,E},
		{B,F,F,F,F,F,F,B,E,E,E,E},
		{B,F,F,F,F,F,B,B,E,E,E,E},
		{B,F,F,F,F,B,B,E,E,E,E,E},
		{B,F,F,F,F,F,B,B,E,B,B,B},
		{B,F,F,B,F,F,F,B,B,B,F,B},
		{B,F,B,B,B,F,F,F,B,F,F,B},
		{B,B,B,E,B,B,F,F,F,F,F,B},
		{E,E,E,E,E,B,B,F,F,F,F,B},
		{E,E,E,E,B,B,F,F,F,F,F,B},
		{E,E,E,E,B,F,F,F,F,F,F,B},
		{E,E,E,E,B,B,B,B,B,B,B,B},

	}
};

// to do: remove it
_image *arrayCursors[] = { &img_defaultCursor, &img_selectCursor, &img_waitingCursor, &img_moveCursor, &img_resizeHeightCursor, &img_resizeWidthCursor, &img_resizeCorner1,  &img_resizeCorner2 };