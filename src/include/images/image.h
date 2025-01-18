#pragma once

# include "klibc/types.h"
# include "drivers/vga.h"
# include "terminal/vga.h"

// to do: make fonts images

typedef struct image
{
	uint32_t width;
	uint32_t height;
	char	 *pixels[];
	// int32_t	 *pixels[];
} _image;

typedef struct font
{
	uint32_t width;
	uint32_t height;
	uint32_t pixels[][16];
} _font;


extern	_vgaMode *g_vgaMode;
extern	_font *g_font;

// FONTS *********************************************************
extern _font g_fontT25x80;
extern _font g_font8x8;
extern _font g_font8x16;
extern _font g_font9x14;
extern _font g_font9x16;



// CURSOR *********************************************************

// B for Border
// F for Fill
// S for Shadow
# define BODER_COLOR	0x07
# define FILL_COLOR		0x00
# define SHADOW_COLOR	0x08
# define DRAW_SHADOW	1

extern _image defaultCursorImage;
extern _image selectCursorImage;
extern _image waitingCursorImage;
extern _image textCursorImage;
extern _image moveCursorImage;
extern _image resizeHeightCursorImage;
extern _image resizeWidthCursorImage;
extern _image resizeCorner1Image; 
extern _image resizeCorner2Image; 

void	drawCursor(_image *image, size_t x, size_t y);
void	drawCharacter(uint8_t character, size_t x, size_t y);