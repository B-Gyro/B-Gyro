# pragma once

# include "klibc/types.h"

# define MAX_KEYBOARD_BUFFER 80
# define MAX_HISTORY_SIZE 128


# define KBD_FLAG_SHIFT 0b00000001
# define KBD_FLAG_CAPS	0b00000010
# define KBD_FLAG_CTRL	0b00000100
# define KBD_FLAG_ALT	0b00001000
# define KBD_FLAG_NUM	0b00010000

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