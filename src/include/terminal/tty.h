#pragma once

# include "terminal/_data.h"
# include "drivers/keyboard.h"

void	initTTY(uint8_t index);
void	putTtyBuffer(void);
void	switchTTY(uint8_t index);
void	clearTTY(uint32_t size);
