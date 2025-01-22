#pragma once

# include "arch/i386/ports/portsIO.h"
# include "terminal/_data.h"

# define VIDEO_ADDRESS		0xb8000

# define VGA_CTRL_REGISTER	0x3d4
# define VGA_DATA_REGISTER	0x3d5
# define VGA_OFFSET_LOW		0x00f
# define VGA_OFFSET_HIGH	0x00e

# define BACKGROUND_BITS	0b11110000
# define FOREGROUND_BITS	0b00001111

# define CURSOR_COLOR		0x07
typedef struct vgaCell _vgaCell; 

void	setVgaColor(uint8_t ansiNbr);
void	clearVGA(bool clearFull);
void	putCellOnVga(_vgaCell cell, uint8_t x, uint8_t y);

void	setCursor( void );
void	updateCursorData(bool draw);

void	decrementCursorY( void );
void	decrementCursorX( void );
void	incrementCursorY( void );
void	incrementCursorX( void );

void	resetCursor( void );
void	updateCursorLoc(size_t x, size_t y);

void	moveCursorLeft( void );
void	moveCursorRight( void );
