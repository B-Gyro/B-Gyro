# pragma once

# include "klibc/types.h"
# include "arch/i386/cpu/descriptorTables.h"


# define MAX_KEYBOARD_BUFFER 80
# define MAX_HISTORY_SIZE 128


# define KEYBOARD_DATA_PORT 0x60
# define KEYBOARD_STATUS_PORT 0x64

typedef enum kbdFlags {
	KBD_FLAG_SHIFT = 0,
	KBD_FLAG_CAPS,
	KBD_FLAG_CTRL,
	KBD_FLAG_ALT,
	KBD_FLAG_NUM,
	KBD_FLAG_NEWLINE
} e_kbdFlags;

typedef void (*onKeyPressHanlder)(uint8_t);
typedef void (*onKeyReleaseHandler)(uint8_t);

typedef struct keyboardViews {
	uint8_t	*keys;
	uint8_t	*shiftedKeys;
} _keyboardViews;

typedef union kbdLayout {
	struct keyboardViews kbdV;
	uint8_t *views[2];
} _kbdLayout;

typedef struct kbdBuffer {
    uint8_t     buffer[MAX_KEYBOARD_BUFFER];
    uint32_t    index;
	uint32_t    size;
} _kbdBuffer;

typedef struct keyboardData {
    _kbdLayout	        layout;
    _kbdBuffer          buffer;
    uint8_t             kbdFlags;
    uint8_t             historyIndex;
    _kbdBuffer          historyBuffer[MAX_HISTORY_SIZE];
    onKeyPressHanlder   keyPressHandler;
    onKeyReleaseHandler keyReleaseHandler;
} _keyboardData;


void	keyboardInit();
void	keyboardSetLayout(_kbdLayout layout);
void	keyboardSetKeyPressHandler(onKeyPressHanlder handler);
void	keyboardSetKeyReleaseHandler(onKeyReleaseHandler handler);

// prompt functions
void	inturruptPrompting();
char	*prompt(char *declare, char *buffer);

// resetting handlers
void	resetKeyReleaseHandler();
void	resetKeyPressHandler();

// getters

uint8_t	getScancode(uint8_t letter);
uint8_t	getLetter(uint8_t scancode);