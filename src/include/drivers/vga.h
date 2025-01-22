# pragma once

# include "klibc/types.h"

/* MR
- [/] http://www.osdever.net/FreeVGA/vga/vga.htm
- [/] http://www.osdever.net/FreeVGA/vga/vgamem.htm
- [X] http://www.osdever.net/FreeVGA/vga/vgareg.htm
*/

struct vgaMode;
struct positionPair;

// function to change the vga mode:
typedef void (*changeModeFct)(void);
// function to put pixel in the selected vga mode:
typedef void (*putPixelFct)(struct positionPair, uint8_t color);
// function to put a character in the selected mdoe:
typedef uint8_t (*putCharPosFct)(char c, size_t x, size_t y);


# define SEQUENCER_REG_ADDR			0x3C4 // http://www.osdever.net/FreeVGA/vga/seqreg.htm
# define SEQUENCER_REG_DATA			0x3C5

# define CRT_CTRL_REG_ADDR_D		0x3D4 // http://www.osdever.net/FreeVGA/vga/crtcreg.htm
# define CRT_CTRL_REG_DATA_D		0x3D5
# define CRT_CTRL_REG_ADDR_B		0x3B4
# define CRT_CTRL_REG_DATA_B		0x3B5

# define GRAPHICS_REG_ADDR			0x3CE // http://www.osdever.net/FreeVGA/vga/graphreg.htm
# define GRAPHICS_REG_DATA			0x3CF

# define ATTRIBUTE_CTRL_REG_ADDR	0x3C0 // http://www.osdever.net/FreeVGA/vga/attrreg.htm
# define ATTRIBUTE_CTRL_REG_DATA	0x3C1

# define MISC_OUTPUT_REG_WR			0x3C2 // http://www.osdever.net/FreeVGA/vga/extreg.htm
# define MISC_OUTPUT_REG_RE			0x3CC

# define FEATURE_CTRL_REG_WR		0x3BA // http://www.osdever.net/FreeVGA/vga/extreg.htm
# define FEATURE_CTRL_REG_RE		0x3CA
# define FEATURE_CTRL_REG_COLOR		0x3DA

// vga graphic registers index
typedef enum vgaGraphicRegisters{
	SET_RESET_REG, 
	ENABLE_SET_RESET_REG,
	COLOR_COMPARE_REG,
	DATA_ROTATE_REG,
	READ_MAP_SELET_REG,
	GRAGHICS_MODE_REG,
	MISC_GRAPHICS_REG,
	COLOR_DONT_CARE_REG,
	BIT_MASK_REG
} e_vgaGraphicRegisters;

// vga sequencer registers index

typedef enum vgaSequencerRegisters{
	RESET_REGISTER,
	CLOCKING_MODE_REGISTER,
	MAP_MASK_REGISTER,
	MAP_SELECT_REGISTER,
	MEMORY_MODE_REGISTER
} e_vgaSequencerRegisters;

typedef struct vgaMode {
	changeModeFct	func;
	// if it's a graphics mode this isn't NULL
	putPixelFct		putPixel;
	// if it's a text mode this isn't NULL
	putCharPosFct	putCharPos;
	char			*VMStart; // video memory start
	size_t			screenHeight;
	size_t			screenWidth;
	uint32_t		maxColors;
} _vgaMode;

typedef struct positionPair {
	size_t	x;
	size_t	y;
} _positionPair;


// fonts:
extern uint8_t g_8x8_font[1024];
extern uint8_t g_8x16_font[4096];

void	changeVGAMode13h(void);
void	changeVGAMode640x480x16(void);
void	changeVGAMode640x480x2(void);
void	changeVGAModeT80x25(void);
void	changeVGAModeT80x50(void);

void	setVideoPlane(uint8_t plane);
void	restorePreviousDumps(void);
void	dumpToVGAPorts(uint8_t *val);
void	setFont(uint8_t *font, uint8_t fontHeight);