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
_keyboardData g_keyboardData;
//------------------------------------------------


// ------------------------------ flag Checks Functions ------------------------------

bool	isCtrlKeyPressed() {
	return BIT_IS_SET(g_keyboardData.kbdFlags, KBD_FLAG_CTRL);
}

bool	isAltKeyPressed() {
	return BIT_IS_SET(g_keyboardData.kbdFlags, KBD_FLAG_ALT);
}

bool	isShiftKeyPressed() {
	return BIT_IS_SET(g_keyboardData.kbdFlags, KBD_FLAG_SHIFT);
}

bool	isCapsLockEnabled() {
	return BIT_IS_SET(g_keyboardData.kbdFlags, KBD_FLAG_CAPS);
}

// ------------------------------ Keyboard handlers Functions ------------------------------

void	keyboardShortcutsHandler(uint8_t scancode){
	uint8_t	letter;

	SERIAL_INFO("shortcuts handler %x", scancode);

	letter = getLetter(scancode);
	switch (letter)
	{
		case 'c':
			SERIAL_INFO("Ctrl + C");
			break;
		default:
			if (isDigit(letter)){
				switchTTY(letter - '0' - 1);
				SERIAL_INFO("Switching to tty %d", letter - '0');
			}
			break;
	}
	return ;
}

void	defaultKeyPressHandler(uint8_t letter) {
	if (letter == '\n')
		return inturruptPrompting();
	if (g_keyboardData.buffer.size < MAX_KEYBOARD_BUFFER)
		g_keyboardData.buffer.buffer[g_keyboardData.buffer.size++] = letter;
	else
		SERIAL_ERR("Buffer is full");

	g_keyboardData.buffer.index++;
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
			default: break;
	};
}

void	handleBackSpace() {
	if (g_keyboardData.buffer.size > 0) {
		g_keyboardData.buffer.buffer[--g_keyboardData.buffer.size] = 0;
		g_keyboardData.buffer.index--;
		putChar('\b');
	}
}

void	handleSpecialKeys(uint8_t scancode){

	switch (scancode)
	{
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
		default: break;
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
	if (isCtrlKeyPressed() || isAltKeyPressed())
		return keyboardShortcutsHandler(scancode);
	
	letter = getLetter(scancode);
	if (letter)
		return	keyPressHandler(letter);

	return handleSpecialKeys(scancode);
}

// ------------------------------ Getters Functions ------------------------------

uint8_t	getScancode(uint8_t letter) {
	uint8_t	*keyboardView;
	bool	selectedView;

	selectedView = isShiftKeyPressed() ^ isCapsLockEnabled();
	keyboardView = g_keyboardData.layout.views[selectedView];

	for (uint8_t i = 0; i < 128; i++)
		if (keyboardView[i] == letter)
			return i;
	return 0;
}

uint8_t getLetter(uint8_t scancode) {
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

void	resetKeyPressHandler() {
	g_keyboardData.keyPressHandler = defaultKeyPressHandler;
}

void	resetKeyReleaseHandler() {
	g_keyboardData.keyReleaseHandler = defaultKeyReleaseHandler;
}

// ------------------------------ Prompt Functions ------------------------------

void inturruptPrompting(){
    BIT_SET(g_keyboardData.kbdFlags, KBD_FLAG_NEWLINE);
}

void	clearKeyboardBuffer(){
    bzero(g_keyboardData.buffer.buffer, g_keyboardData.buffer.size);
    g_keyboardData.buffer.index = 0;
	g_keyboardData.buffer.size = 0;
}

char	*prompt(char *declare, char *buffer) {

	if (declare)
        VGA_PRINT("%s> ", declare);
    while (!BIT_IS_SET(g_keyboardData.kbdFlags, KBD_FLAG_NEWLINE))
        asm volatile("" : : : "memory");

    BIT_RESET(g_keyboardData.kbdFlags, KBD_FLAG_NEWLINE);
    strlcpy(buffer, (char *)g_keyboardData.buffer.buffer, g_keyboardData.buffer.size);
    //addToHistory();
    clearKeyboardBuffer();
    VGA_PRINT("\n\r");
    return buffer;
}

// ------------------------------ Initialization Functions ------------------------------

void keyboardInit() {
	g_keyboardData.buffer.index = 0;
	g_keyboardData.buffer.size = 0;
	g_keyboardData.historyIndex = 0;
	g_keyboardData.kbdFlags = 0;

	keyboardSetLayout(g_kbdQwerty);
	keyboardSetKeyPressHandler(defaultKeyPressHandler);
	keyboardSetKeyReleaseHandler(defaultKeyReleaseHandler);
	setIRQHandler(1, keyboardInterruptHandler);
}