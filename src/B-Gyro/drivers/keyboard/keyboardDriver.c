# include "drivers/keyboard.h"
# include "klibc/print.h"
# include "klibc/strings.h"
# include "klibc/memory.h"
# include "arch/i386/ports/portsIO.h"
# include "terminal/tty.h"


//--------------- Keyboard Layouts ---------------
extern _kbdLayout g_kbdQwerty;
extern _kbdLayout g_kbdAzerty;
//------------------------------------------------

//--------------- Keyboard Data -------------------
_keyboardData			g_keyboardData;
extern _shortcut		g_shortcuts[MAX_SHORTCUTS];
extern _terminal		g_terminal;
//------------------------------------------------


// ------------------------------ flag Checks Functions ------------------------------

bool	isCtrlKeyPressed(void) {
	return BIT_IS_SET(g_keyboardData.kbdFlags, KBD_FLAG_CTRL);
}

bool	isAltKeyPressed(void) {
	return BIT_IS_SET(g_keyboardData.kbdFlags, KBD_FLAG_ALT);
}

bool	isShiftKeyPressed(void) {
	return BIT_IS_SET(g_keyboardData.kbdFlags, KBD_FLAG_SHIFT);
}

bool	isCapsLockEnabled(void) {
	return BIT_IS_SET(g_keyboardData.kbdFlags, KBD_FLAG_CAPS);
}

// ------------------------------ Keyboard handlers Functions ------------------------------

void	keyboardShortcutsHandler(uint8_t scancode){
	uint8_t	letter, cleanedKbdFlags;
	bool	isMirroredShortcut;


	letter = keyboardGetLetter(scancode);
	// just keyboard flags without caps, and new line ... things we don't need to check:
	cleanedKbdFlags = g_keyboardData.kbdFlags & ~(1 << KBD_FLAG_CAPS | 1 << KBD_FLAG_NEWLINE | 1 << KBD_FLAG_NUM);

	for (size_t i = 0; (i < MAX_SHORTCUTS) && (g_shortcuts[i].handler != NULL); i++) {
		isMirroredShortcut = (cleanedKbdFlags == g_shortcuts[i].flagedModifiers);
		if ((g_shortcuts[i].key == toUpperCase(letter)) && isMirroredShortcut)
			return g_shortcuts[i].handler();
	}
	// special Case
	if (isDigit(letter)){
		switchTTY(letter - '0' - 1);
		SERIAL_INFO("Switching to tty %d", letter - '0');
	}
	return ;
}

void	defaultKeyPressHandler(uint8_t letter) {
	if (letter == '\n')
		return inturruptPrompting();
	if (g_keyboardData.buffer->size >= MAX_KEYBOARD_BUFFER) {
		SERIAL_ERR("Buffer is full");
		return ;
	}
	g_keyboardData.buffer->buffer[g_keyboardData.buffer->size++] = letter;
	g_keyboardData.buffer->index++;
	putChar(letter);
}

void	defaultKeyReleaseHandler(uint8_t scancode) {
	switch (scancode)
	{
		case 0xAA:
        	BIT_RESET(g_keyboardData.kbdFlags, KBD_FLAG_SHIFT);
			break;
		case 0xB6:
    	    BIT_RESET(g_keyboardData.kbdFlags, KBD_FLAG_SHIFT);
			break;
		case 0x9D:
    	    BIT_RESET(g_keyboardData.kbdFlags, KBD_FLAG_CTRL);
			break;
		case 0xB8:
			BIT_RESET(g_keyboardData.kbdFlags, KBD_FLAG_ALT);
			break;
		default:
			break;
	};
}

void	handleBackSpace(void) {
	if (g_keyboardData.buffer->size > 0) {
		g_keyboardData.buffer->buffer[--g_keyboardData.buffer->size] = 0;
		g_keyboardData.buffer->index--;
		putChar('\b');
	}
}

void	handleSpecialKeys(uint8_t scancode){

	switch (scancode)
	{
		case CURSOR_DOWN:
			getHistory(CURSOR_DOWN);
			break;
		case CURSOR_UP:
			getHistory(CURSOR_UP);
			break;
		case 0x0E:
			handleBackSpace();
			break;
		case 0x2A:
			BIT_SET(g_keyboardData.kbdFlags, KBD_FLAG_SHIFT);
			break;
		case 0x36:
			BIT_SET(g_keyboardData.kbdFlags, KBD_FLAG_SHIFT);
			break;
		case 0x3A:
			if (isCapsLockEnabled())
				BIT_RESET(g_keyboardData.kbdFlags, KBD_FLAG_CAPS);
			else
				BIT_SET(g_keyboardData.kbdFlags, KBD_FLAG_CAPS);
			break;
		case 0x1D:
			BIT_SET(g_keyboardData.kbdFlags, KBD_FLAG_CTRL);
			break;
		case 0x38:
			BIT_SET(g_keyboardData.kbdFlags, KBD_FLAG_ALT);
			break;
		default:
			break;
	};
}

void	keyboardInterruptHandler(_registers r) {
	uint8_t				scancode, letter;
	onKeyPressHanlder	keyPressHandler;
	onKeyReleaseHandler	keyReleaseHandler;

	(void) r;

	scancode = portByteIn(KEYBOARD_DATA_PORT);

	if (!g_keyboardData.keyPressHandler)
		resetKeyPressHandler();
	if (!g_keyboardData.keyReleaseHandler)
		resetKeyReleaseHandler();

	keyPressHandler = g_keyboardData.keyPressHandler;
	keyReleaseHandler = g_keyboardData.keyReleaseHandler;

	if (scancode & 0x80)
		return keyReleaseHandler(scancode);
	letter = keyboardGetLetter(scancode);
	if ((isCtrlKeyPressed() || isAltKeyPressed()) && letter)
		return keyboardShortcutsHandler(scancode);
	
	if (letter)
		return	keyPressHandler(letter);

	return handleSpecialKeys(scancode);
}

// ------------------------------ Getters Functions ------------------------------

uint8_t	keyboardGetScancode(uint8_t letter) {
	uint8_t	*keyboardView;
	bool	selectedView;

	selectedView = isShiftKeyPressed() ^ isCapsLockEnabled();
	keyboardView = g_keyboardData.layout.views[selectedView];

	for (uint8_t i = 0; i < 128; i++)
		if (keyboardView[i] == letter)
			return i;
	return 0;
}

uint8_t	keyboardGetLetter(uint8_t scancode) {
	uint8_t	*keyboardView;
	bool	selectedView;

	selectedView = isShiftKeyPressed() ^ isCapsLockEnabled();
	keyboardView = g_keyboardData.layout.views[selectedView];

	return keyboardView[scancode];
}

// ------------------------------ Setters Functions ------------------------------

void keyboardSetLayout(_kbdLayout layout) {
	g_keyboardData.layout = layout;
}

void keyboardSetKeyPressHandler(onKeyPressHanlder handler) {
	g_keyboardData.keyPressHandler = handler;
}

void keyboardSetKeyReleaseHandler(onKeyReleaseHandler handler) {
	g_keyboardData.keyReleaseHandler = handler;
}

// ------------------------------ Resetters Functions ------------------------------

void	resetKeyPressHandler(void) {
	g_keyboardData.keyPressHandler = defaultKeyPressHandler;
}

void	resetKeyReleaseHandler(void) {
	g_keyboardData.keyReleaseHandler = defaultKeyReleaseHandler;
}

// ------------------------------ Prompt Functions ------------------------------

void inturruptPrompting(void){
    BIT_SET(g_keyboardData.kbdFlags, KBD_FLAG_NEWLINE);
}

void	keyboardClearBuffer(void){
    bzero(g_keyboardData.buffer->buffer, g_keyboardData.buffer->size);
    g_keyboardData.buffer->index = 0;
	g_keyboardData.buffer->size = 0;
}

void	keyboardSetBuffer(_kbdBuffer *currentTTYBuffer, bool clearBuffer){
	g_keyboardData.buffer = currentTTYBuffer;

	if (clearBuffer)
		keyboardClearBuffer();
}


char	*prompt(char *declare, char *buffer) {
	if (declare)
        VGA_PRINT("%s>"COLOR_DEFAULT" ", declare);
    while (!BIT_IS_SET(g_keyboardData.kbdFlags, KBD_FLAG_NEWLINE))
        asm volatile("" : : : "memory");

    BIT_RESET(g_keyboardData.kbdFlags, KBD_FLAG_NEWLINE);
    strlcpy(buffer, (char *)g_keyboardData.buffer->buffer, g_keyboardData.buffer->size);
    addToHistory();
    keyboardClearBuffer();
    VGA_PRINT("\n\r");
    return buffer;
}

// ------------------------------ Initialization Functions ------------------------------

void keyboardInit(void) {

	keyboardSetBuffer(&g_terminal.currentTTY->keyboardBuffer, 1);
	g_keyboardData.kbdFlags = 0;

	keyboardSetLayout(g_kbdQwerty);
	keyboardSetKeyPressHandler(defaultKeyPressHandler);
	keyboardSetKeyReleaseHandler(defaultKeyReleaseHandler);
	setIRQHandler(KEYBOARD_IRQ, keyboardInterruptHandler);
}