#pragma once

# include "arch/i386/ports/portsIO.h"
# include "terminal/_data.h"

# define VIDEO_ADDRESS		CURRENT_TTY->mode->VMStart

# define VGA_CTRL_REGISTER	0x3d4
# define VGA_DATA_REGISTER	0x3d5
# define VGA_OFFSET_LOW		0x00f
# define VGA_OFFSET_HIGH	0x00e

# define BACKGROUND_BITS	0b11110000
# define FOREGROUND_BITS	0b00001111

# define CURSOR_COLOR		0x07

# define VGA_BLACK			0x00
# define VGA_BLUE			0x01
# define VGA_GREEN			0x02
# define VGA_CYAN			0x03
# define VGA_RED			0x04
# define VGA_MAGENTA		0x05
# define VGA_YELLOW			0x06
# define VGA_WHITE			0x07
# define VGA_BRIGHT_BLACK	0x08
# define VGA_BRIGHT_BLUE	0x09
# define VGA_BRIGHT_GREEN	0x0A
# define VGA_BRIGHT_CYAN	0x0B
# define VGA_BRIGHT_RED		0x0C
# define VGA_BRIGHT_MAGENTA	0x0D
# define VGA_BRIGHT_YELLOW	0x0E
# define VGA_BRIGHT_WHITE	0x0F

typedef struct vgaCell _vgaCell; 

void	setVgaColor(uint8_t ansiNbr);
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
