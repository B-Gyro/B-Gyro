#pragma once

# include "klibc/types.h"
# include "drivers/vga.h"
// # include "terminal/vga.h"
# include "images/RGB.h"

typedef struct vgaCell _vgaCell; 


typedef struct image
{
	uint32_t width;
	uint32_t height;
	// to do: change this later
	int32_t	 pixels[][260];
} _image;

// FONTS *********************************************************
extern _image g_fontText;
extern _image g_font8x8;
extern _image g_font8x16;
extern _image g_font9x14;
extern _image g_font9x16;

extern _image img_logo; 



// CURSOR *********************************************************

# define BODER_COLOR	0x07
# define FILL_COLOR		0x00
# define SHADOW_COLOR	0x08
# define DRAW_SHADOW	1

extern _image img_defaultCursor;
extern _image img_selectCursor;
extern _image img_waitingCursor;
extern _image img_moveCursor;
extern _image img_resizeHeightCursor;
extern _image img_resizeWidthCursor;
extern _image img_resizeCorner1; 
extern _image img_resizeCorner2; 

void	drawCharacter(_vgaCell cell, size_t x, size_t y);
void	drawFilledRectangle(size_t x, size_t y, size_t width, size_t height, uint16_t color);
void	drawImage(_image *image, size_t x, size_t y);
void	drawLine(_positionPair start, _positionPair end, uint8_t color);
