#include "klibc/types.h"
#include "drivers/keyboard.h"

uint8_t g_KeyboardAzerty[128] =
{
    0,  27, '&', 130, '"', '\'', '(', '-', 138, '_', 135, 133, ')', '=',
	0,	/* <-- back space */   
  '\t',		/* <-- Tab */
  'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n',     
    0, /* <-- control key */
  'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 151, 0,  0, '*', 'w', 'x', 'c', 'v', 'b', 'n', ',', ';', ':', '!',   0,
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   '<',
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

uint8_t g_SKeyboardAzerty[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 248, '+',
	0,	/* <-- back space */   
  '\t',		/* <-- Tab */
  'A', 'Z', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 126, 156, '\n',     
    0, /* <-- control key */
  'Q', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', '%', 0,  0, 230, 'W', 'X', 'C', 'V', 'B', 'N', '?', '.', '/', 21,   0,
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   '>',
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

_kbdLayout g_kbdAzerty = {.kbdV.keys = g_KeyboardAzerty, .kbdV.shiftedKeys = g_SKeyboardAzerty};