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
	int32_t	 pixels[][640];
} _image;

// FONTS *********************************************************
extern _image g_fontText;
extern _image g_font8x8;
extern _image g_font8x16;
extern _image g_font9x14;
extern _image g_font9x16;

extern _image img_logo; 
extern _image img_42;


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

// PIXEL *********************************************************

# define DRAW_PIXEL		0
# define STORE_PIXEL	1

extern void	(*g_pixelsOperation)(_positionPair, uint8_t, bool);

void	drawStoredPixels(uint8_t color, bool reset);
void	switchPixelsMode(bool b);

void	drawCharacterByPixel(_vgaCell cell, size_t x, size_t y);
void	drawCharacterBy8Pixels(_vgaCell cell, size_t x, size_t y);
void	drawImage(_image *image, size_t x, size_t y);

void	drawLine(_positionPair start, _positionPair end, uint8_t color);
void	drawHorizontalLine(size_t y, size_t x0, size_t x1, uint8_t color);
void	drawVerticalLine(size_t x, size_t y0, size_t y1, uint8_t color);


void	drawFilledTriangle(_positionPair p1, _positionPair p2, _positionPair p3, uint8_t color);
void	drawFilledRectangle(_positionPair pos, size_t width, size_t height, uint16_t color);
// void	drawFilledCircle(_positionPair pos, size_t radius, uint8_t color);
void	drawFilledSquare(_positionPair pos, size_t width, uint16_t color);
void	drawFilledRhombus(_positionPair pos, size_t diagX, size_t diagY, uint8_t color);

void	drawTriangle(_positionPair p1, _positionPair p2, _positionPair p3, uint8_t color);
void	drawRectangle(_positionPair pos, size_t width, size_t height, uint16_t color);
// void	drawCircle(_positionPair pos, size_t radius, uint8_t color);
void	drawSquare(_positionPair pos, size_t width, uint16_t color);
void	drawRhombus(_positionPair pos, size_t diagX, size_t diagY, uint8_t color);
