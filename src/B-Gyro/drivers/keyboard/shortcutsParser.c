# include "drivers/keyboard.h"
# include "klibc/strings.h"
# include "klibc/print.h"

_shortcut	g_shortcuts[MAX_SHORTCUTS];

static bool	setkeyFlag(char c, _shortcut *shorty){
	switch (c) {
		case 'a':
			BIT_SET(shorty->flagedModifiers, KBD_FLAG_ALT);
			break;
		case 'c':
			BIT_SET(shorty->flagedModifiers, KBD_FLAG_CTRL);
			break;
		case 's':
			BIT_SET(shorty->flagedModifiers, KBD_FLAG_SHIFT);
			break;
		default:
			return 0;
	}
	return 1;
}

static bool	flagkeyModifier(char *key, _shortcut *shorty){
	size_t		keyLen;
	static char *modifers[] = {"ctrl", "alt", "shift", NULL};

	keyLen = strlen(key);
	for (size_t i = 0; modifers[i]; i++) {
		if (!strncmp(modifers[i], key, keyLen))
			return setkeyFlag(key[0], shorty);
	}
	return 0;
}

void	debugShortcut(_shortcut *shorty){
	SERIAL_INFO("Setting Shortcut: ");
	if (BIT_IS_SET(shorty->flagedModifiers, KBD_FLAG_CTRL))
		SERIAL_PRINT("Ctrl");
	if (BIT_IS_SET(shorty->flagedModifiers, KBD_FLAG_SHIFT))
		SERIAL_PRINT("+Shift");
	if (BIT_IS_SET(shorty->flagedModifiers, KBD_FLAG_ALT))
		SERIAL_PRINT("+Alt");
	SERIAL_PRINT("+%c\n", shorty->key);
	SERIAL_SUCC("Shortcut Set");
}

void	setShortcut(char *shortcutFormula, onShortcutHandler func){
	char			*currentShortcutKey;
	bool			isCorrectShortcut;
	static uint8_t	index;
	_shortcut		*shorty;

	shorty = &g_shortcuts[index];
	isCorrectShortcut = TRUE;
	currentShortcutKey = strtok(shortcutFormula, "+");
	while (currentShortcutKey && isCorrectShortcut) {
		// if it's something like shift/ctrl/alt ...
		if (strlen(currentShortcutKey) > 1)
			isCorrectShortcut = flagkeyModifier(currentShortcutKey, shorty);
		// else it should be a character:
		else if (isPrintable(currentShortcutKey[0]))
			shorty->key = toUpperCase(currentShortcutKey[0]);
		// if neither than it's a problem and should be flaged
		else
			isCorrectShortcut = FALSE;
		currentShortcutKey = strtok(NULL, "+");
	}
	if (isCorrectShortcut)
		shorty->handler = func;
	debugShortcut(shorty);
	index++;
}