#include "terminal/tty.h"
#include "klibc/print.h"
#include "terminal/vga.h"
#include "klibc/memory.h"
#include "klibc/strings.h"
#include "drivers/keyboard.h"
#include "arch/i386/ports/portsIO.h"

uint32_t	g_maxKeyboadBuffer = DEFAULT_MAX_KEYBOARD_BUFFER;

//--------------- Keyboard Layouts ---------------
extern _kbdLayout g_kbdQwerty;
extern _kbdLayout g_kbdAzerty;
//------------------------------------------------

//--------------- Keyboard Data -------------------
_keyboardData g_keyboardData;
extern _shortcut g_shortcuts[MAX_SHORTCUTS];
extern _terminal g_terminal;
//------------------------------------------------

// ------------------------------ flag Checks Functions ------------------------------

bool isCtrlKeyPressed(void){
	return BIT_IS_SET(g_keyboardData.kbdFlags, KBD_FLAG_CTRL);
}

bool isAltKeyPressed(void){
	return BIT_IS_SET(g_keyboardData.kbdFlags, KBD_FLAG_ALT);
}

bool isShiftKeyPressed(void){
	return BIT_IS_SET(g_keyboardData.kbdFlags, KBD_FLAG_SHIFT);
}

bool isCapsLockEnabled(void){
	return BIT_IS_SET(g_keyboardData.kbdFlags, KBD_FLAG_CAPS);
}

// ------------------------------ Keyboard handlers Functions ------------------------------

void keyboardShortcutsHandler(uint8_t scancode)
{
	uint8_t letter, cleanedKbdFlags;
	bool isMirroredShortcut;

	letter = keyboardGetLetter(scancode);
	// just keyboard flags without caps, and new line ... things we don't need to check:
	cleanedKbdFlags = g_keyboardData.kbdFlags & ~(1 << KBD_FLAG_CAPS | 1 << KBD_FLAG_NEWLINE | 1 << KBD_FLAG_NUM);

	for (size_t i = 0; (i < MAX_SHORTCUTS) && (g_shortcuts[i].handler != NULL); i++){
		isMirroredShortcut = (cleanedKbdFlags == g_shortcuts[i].flagedModifiers);
		if ((g_shortcuts[i].key == toUpperCase(letter)) && isMirroredShortcut)
			return g_shortcuts[i].handler();
	}
	// special Case
	if (isDigit(letter)){
		switchTTY(letter - '0' - 1);
		SERIAL_INFO("Switching to tty %d", letter - '0');
	}
	return;
}

void	insertCharacter(uint8_t letter){
	ssize_t bufferIndex, bufferSize;
	_kbdBuffer	*kbdBuffer;

	bufferIndex = g_keyboardData.buffer->index;
	bufferSize = g_keyboardData.buffer->size;
	kbdBuffer = g_keyboardData.buffer;

	if (bufferIndex != bufferSize)
		memmove(&kbdBuffer->buffer[bufferIndex + 1], &kbdBuffer->buffer[bufferIndex], bufferSize - bufferIndex);
	kbdBuffer->buffer[g_keyboardData.buffer->index] = letter;
	kbdBuffer->size++;
	kbdBuffer->index++;
}

void handleBackSpace(void) {
	ssize_t bufferIndex, bufferSize;
	_kbdBuffer	*kbdBuffer;


	kbdBuffer = g_keyboardData.buffer;
	bufferIndex = kbdBuffer->index;
	bufferSize = kbdBuffer->size;
	if (bufferIndex > 0) {
		if (bufferIndex != bufferSize)
			memmove(&kbdBuffer->buffer[bufferIndex - 1], &kbdBuffer->buffer[bufferIndex], bufferSize - bufferIndex);
		kbdBuffer->buffer[bufferSize] = 0;
		kbdBuffer->index--;
		kbdBuffer->size--;
		putChar('\b');
	}
}

void handleSpecialKeys(uint8_t scancode){
	switch (scancode) {
		case ARROW_LEFT:
			moveCursorLeft();
			break;
		case ARROW_RIGHT:
			moveCursorRight();
			break;
		case ARROW_DOWN:
			getHistory(ARROW_DOWN);
			break;
		case ARROW_UP:
			getHistory(ARROW_UP);
			break;
		case BACK_SPACE:
			handleBackSpace();
			break;
		case LEFT_SHIFT:
			BIT_SET(g_keyboardData.kbdFlags, KBD_FLAG_SHIFT);
			break;
		case RIGHT_SHIFT:
			BIT_SET(g_keyboardData.kbdFlags, KBD_FLAG_SHIFT);
			break;
		case CAPS_LOCK:
			if (isCapsLockEnabled())
				BIT_RESET(g_keyboardData.kbdFlags, KBD_FLAG_CAPS);
			else
				BIT_SET(g_keyboardData.kbdFlags, KBD_FLAG_CAPS);
			break;
		case CTRL:
			BIT_SET(g_keyboardData.kbdFlags, KBD_FLAG_CTRL);
			break;
		case ALT:
			BIT_SET(g_keyboardData.kbdFlags, KBD_FLAG_ALT);
			break;
		default:
			break;
	};
}

void	defaultKeyPressHandler(uint8_t letter, _scanCode scancode){

	if (!letter)
		return handleSpecialKeys(scancode);
	if (letter == '\n')
		return interruptPrompting();
	if (g_keyboardData.buffer->size >= MAX_KEYBOARD_BUFFER){
		SERIAL_ERR("Buffer is full");
		return;
	}
	insertCharacter(letter);
	putChar(letter);
}

void	passwordKeyHandler(uint8_t letter, _scanCode scancode){
	(void)scancode;
	if (letter == '\n')
		return interruptPrompting();
	
	if (g_keyboardData.buffer->size >= MAX_KEYBOARD_BUFFER){
		SERIAL_ERR("Buffer is full");
		return;
	}
	insertCharacter(letter);
	putChar('*');
}

void defaultKeyReleaseHandler(_scanCode scancode){

	REMOVE_RELEASE_FLAG(scancode);
	switch (scancode) {
		case LEFT_SHIFT:
			BIT_RESET(g_keyboardData.kbdFlags, KBD_FLAG_SHIFT);
			break;
		case RIGHT_SHIFT:
			BIT_RESET(g_keyboardData.kbdFlags, KBD_FLAG_SHIFT);
			break;
		case CTRL:
			BIT_RESET(g_keyboardData.kbdFlags, KBD_FLAG_CTRL);
			break;
		case ALT:
			BIT_RESET(g_keyboardData.kbdFlags, KBD_FLAG_ALT);
			break;
		default:
			break;
	};
}

void keyboardInterruptHandler(_registers r){
	uint8_t scancode, letter;
	onKeyPressHanlder keyPressHandler;
	onKeyReleaseHandler keyReleaseHandler;

	(void)r;

	scancode = portByteIn(KEYBOARD_DATA_PORT);

	if (!g_keyboardData.keyPressHandler)
		keyboardResetKeyPressHandler();
	if (!g_keyboardData.keyReleaseHandler)
		keyboardResetKeyReleaseHandler();

	keyPressHandler = g_keyboardData.keyPressHandler;
	keyReleaseHandler = g_keyboardData.keyReleaseHandler;

	if (scancode & 0x80)
		return keyReleaseHandler(scancode);
	letter = keyboardGetLetter(scancode);
	if ((isCtrlKeyPressed() || isAltKeyPressed()) && letter)
		return keyboardShortcutsHandler(scancode);
	keyPressHandler(letter, scancode);
}

// ------------------------------ Getters Functions ------------------------------

uint8_t keyboardGetScancode(uint8_t letter){
	uint8_t *keyboardView;
	bool selectedView;

	selectedView = isShiftKeyPressed() ^ isCapsLockEnabled();
	keyboardView = g_keyboardData.layout.views[selectedView];

	for (uint8_t i = 0; i < 128; i++)
		if (keyboardView[i] == letter)
			return i;
	return 0;
}

uint8_t keyboardGetLetter(uint8_t scancode){
	uint8_t *keyboardView;
	bool selectedView;

	selectedView = isShiftKeyPressed() ^ isCapsLockEnabled();
	keyboardView = g_keyboardData.layout.views[selectedView];

	return keyboardView[scancode];
}

// ------------------------------ Setters Functions ------------------------------

void keyboardSetLayout(_kbdLayout layout){
	g_keyboardData.layout = layout;
}

void keyboardSetKeyPressHandler(onKeyPressHanlder handler){
	g_keyboardData.keyPressHandler = handler;
}

void keyboardSetKeyReleaseHandler(onKeyReleaseHandler handler){
	g_keyboardData.keyReleaseHandler = handler;
}

// ------------------------------ Resetters Functions ------------------------------

void keyboardResetKeyPressHandler(void){
	g_keyboardData.keyPressHandler = defaultKeyPressHandler;
}

void keyboardResetKeyReleaseHandler(void){
	g_keyboardData.keyReleaseHandler = defaultKeyReleaseHandler;
}

// ------------------------------ Prompt Functions ------------------------------

void interruptPrompting(void){
	BIT_SET(g_keyboardData.kbdFlags, KBD_FLAG_NEWLINE);
}

void keyboardClearBuffer(void){
	bzero(g_keyboardData.buffer->buffer, g_keyboardData.buffer->size);
	g_keyboardData.buffer->index = 0;
	g_keyboardData.buffer->size = 0;
}

void keyboardSetBuffer(_kbdBuffer *currentTTYBuffer, bool clearBuffer){
	g_keyboardData.buffer = currentTTYBuffer;

	if (clearBuffer)
		keyboardClearBuffer();
}

char *prompt(char *promtMessage, char *buffer){
	if (promtMessage)
		VGA_PRINT("%s" COLOR_DEFAULT" ", promtMessage);
	while (!BIT_IS_SET(g_keyboardData.kbdFlags, KBD_FLAG_NEWLINE))
		asm volatile("" : : : "memory");

	BIT_RESET(g_keyboardData.kbdFlags, KBD_FLAG_NEWLINE);
	size_t	i;
	for (i = 0; i < g_keyboardData.buffer->size; i++){
		if ((g_keyboardData.buffer->buffer[i] != ' ') && (g_keyboardData.buffer->buffer[i] != '\t'))
			break;
	}
	strlcpy(buffer, (char *)g_keyboardData.buffer->buffer + i, g_keyboardData.buffer->size - i);
	if (g_keyboardData.buffer->size - i)
		addToHistory();
	keyboardClearBuffer();
	FILL_BUFFER("\n\r");
	updateCursorData(0);
	return buffer;
}

char *prompt_(char *promtMessage, char *buffer){
	if (promtMessage)
		VGA_PRINT("%s" COLOR_DEFAULT" ", promtMessage);
	while (!BIT_IS_SET(g_keyboardData.kbdFlags, KBD_FLAG_NEWLINE))
		asm volatile("" : : : "memory");

	BIT_RESET(g_keyboardData.kbdFlags, KBD_FLAG_NEWLINE);

	strlcpy(buffer, (char *)g_keyboardData.buffer->buffer, g_keyboardData.buffer->size);
	VGA_PRINT("\n");
	if (CURRENT_TTY->mode->putPixel)
		drawFilledRectangle((_positionPair){CURRENT_TTY->cursorX * FONT_WIDTH, CURRENT_TTY->cursorY * FONT_HEIGHT}, FONT_WIDTH, FONT_HEIGHT, g_currentBackGroundColor);
	
	keyboardClearBuffer();

	return buffer;
}

// ------------------------------ Initialization Functions ------------------------------

void keyboardInit(void){

	keyboardSetBuffer(&CURRENT_TTY->keyboardBuffer, 1);
	g_keyboardData.kbdFlags = 0;

	keyboardSetLayout(g_kbdQwerty);
	// keyboardSetLayout(g_kbdAzerty);
	keyboardSetKeyPressHandler(defaultKeyPressHandler);
	keyboardSetKeyReleaseHandler(defaultKeyReleaseHandler);
	setIRQHandler(KEYBOARD_IRQ, keyboardInterruptHandler);
}