#pragma once


typedef enum scanCodes {
	ARROW_UP		= 0x48,
	ARROW_DOWN		= 0x50,
	ARROW_RIGHT		= 0x4D,
	ARROW_LEFT		= 0x4B,
	BACK_SPACE		= 0x0E,
	LEFT_SHIFT  	= 0x2A,
	RIGHT_SHIFT 	= 0x36,
	CAPS_LOCK		= 0x3A,
	CTRL			= 0x1D,
	ALT				= 0x38,
	ESCAPE			= 0x01,
}e_scanCodes;

# define REMOVE_RELEASE_FLAG(VAR) (VAR &= ~(0x80))