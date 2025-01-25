#pragma once

# include "klibc/types.h"

typedef struct rgb
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} _rgb;

extern _rgb g_rgb16[16];

void    setPalette(uint8_t index, _rgb *rgb);
void    setDefaultPalette( void );
