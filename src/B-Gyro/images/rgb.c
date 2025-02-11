# include "klibc/types.h"
# include "images/RGB.h"
#include "terminal/vga.h"

_rgb g_rgb16[16] = {
	{   0,   0,   0 },	// BLACK
	{   0,   0, 170 },	// BLUE
	{   0, 170,   0 },	// GREEN
	{   0, 170, 170 },	// CYAN
	{ 170,   0,   0 },	// RED
	{ 170,   0, 170 },	// MAGENTA
	{ 170,  85,   0 },	// YELLOW
	{ 170, 170, 170 },	// WHITE
	{  85,  85,  85 },	// BRIGHT BLACK
	{  85,  85, 255 },	// BRIGHT BLUE
	{  85, 255,  85 },	// BRIGHT GREEN
	{  85, 255, 255 },	// BRIGHT CYAN
	{ 255,  85,  85 },	// BRIGHT RED
	{ 255,  85, 255 },	// BRIGHT MAGENTA
	{ 255, 255,  85 },	// BRIGHT YELLOW
	{ 255, 255, 255 }   // BRIGHT WHITE
};

void    setDefaultPalette( void ) {
    for (uint8_t i = 0; i < 16; i++)
        setPalette(i, &g_rgb16[i]);
}

void    setPalette(uint8_t index, _rgb *rgb) {
    portByteOut(0x3C8, index);

    portByteOut(0x3C9, rgb->r);
    portByteOut(0x3C9, rgb->g);
    portByteOut(0x3C9, rgb->b);
}
