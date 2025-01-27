#include "klibc/print.h"
#include "drivers/vga.h"
#include "klibc/memory.h"
#include "arch/i386/ports/portsIO.h"



void setVideoPlane(uint8_t plane) {
    uint8_t currentMask;

    // Select the Map Mask register
    portByteOut(SEQUENCER_REG_ADDR, MAP_MASK_REGISTER);
    
    // Read the current value and set only the desired plane bit
    currentMask = portByteIn(SEQUENCER_REG_DATA) & 0xF0;  // Preserve unrelated bits
    portByteOut(SEQUENCER_REG_DATA, currentMask | (1 << plane));
}


void dumpToVGAPorts(uint8_t *val) {
	portByteOut(MISC_OUTPUT_REG_WR, *val);
	val++;
	for (uint8_t i = 0; i < 5; i++) {
		portByteOut(SEQUENCER_REG_ADDR, i);
		portByteOut(SEQUENCER_REG_DATA, *val);
		val++;
	}
	portByteOut(CRT_CTRL_REG_ADDR_D, 0x03);
	portByteOut(CRT_CTRL_REG_DATA_D, portByteIn(CRT_CTRL_REG_DATA_D) | 0x80);
	portByteOut(CRT_CTRL_REG_ADDR_D, 0x11);
	portByteOut(CRT_CTRL_REG_DATA_D, portByteIn(CRT_CTRL_REG_DATA_D) & ~0x80);
	val[0x03] |= 0x80;
	val[0x11] &= ~0x80;
	for (uint8_t i = 0; i < 25; i++) {
		portByteOut(CRT_CTRL_REG_ADDR_D, i);
		portByteOut(CRT_CTRL_REG_DATA_D, *val);
		val++;
	}
	for (uint8_t i = 0; i < 9; i++) {
		portByteOut(GRAPHICS_REG_ADDR, i);
		portByteOut(GRAPHICS_REG_DATA, *val);
		val++;
	}
	for (uint8_t i = 0; i < 21; i++) {
		(void)portByteIn(FEATURE_CTRL_REG_COLOR);
		portByteOut(ATTRIBUTE_CTRL_REG_ADDR, i);
		portByteOut(ATTRIBUTE_CTRL_REG_DATA, *val);
		val++;
	}
	(void)portByteIn(FEATURE_CTRL_REG_COLOR);
	portByteOut(ATTRIBUTE_CTRL_REG_ADDR, 0x20);
}

void setFontOld(uint8_t *font, uint8_t fontHeight) {
    // Sequencer registers:
    uint8_t mapMaskRegister, memoryModeRegister;
    // Graphics registers:
    uint8_t readMapSelectRegister, graphicsModeRegister, miscGraphicsRegister;

    // Save current state of the sequencer registers
    portByteOut(SEQUENCER_REG_ADDR, MAP_MASK_REGISTER);
    mapMaskRegister = portByteIn(SEQUENCER_REG_DATA);

    portByteOut(SEQUENCER_REG_ADDR, MEMORY_MODE_REGISTER);
    memoryModeRegister = portByteIn(SEQUENCER_REG_DATA);
    // Set flat addressing:
    portByteOut(SEQUENCER_REG_DATA, memoryModeRegister | 0x04);

    // Save current state of the graphics registers
    portByteOut(GRAPHICS_REG_ADDR, MAP_SELECT_REGISTER);
    readMapSelectRegister = portByteIn(GRAPHICS_REG_DATA);

    portByteOut(GRAPHICS_REG_ADDR, GRAGHICS_MODE_REG);
    graphicsModeRegister = portByteIn(GRAPHICS_REG_DATA);
    portByteOut(GRAPHICS_REG_DATA, graphicsModeRegister & ~0x10); // Turn off even-odd addressing

    portByteOut(GRAPHICS_REG_ADDR, MISC_GRAPHICS_REG);
    miscGraphicsRegister = portByteIn(GRAPHICS_REG_DATA);
    portByteOut(GRAPHICS_REG_DATA, miscGraphicsRegister & ~0x02); // Turn off even-odd addressing

    // Write the font to plane P4 (assuming they are P1, P2, P4, P8)
    setVideoPlane(2);

    // Write to font 0:
    uint8_t *fontMemory = (uint8_t *)0x4000;
    for (uint16_t i = 0; i < 256; i++) {
        memcpy(fontMemory + i * 32, font, fontHeight);
        font += fontHeight;
    }
    // Restore the video plane
    setVideoPlane(0);

    // Restore registers:
    portByteOut(SEQUENCER_REG_ADDR, MAP_MASK_REGISTER);
    portByteOut(SEQUENCER_REG_DATA, mapMaskRegister);
    portByteOut(SEQUENCER_REG_ADDR, MEMORY_MODE_REGISTER);
    portByteOut(SEQUENCER_REG_DATA, memoryModeRegister);
    portByteOut(GRAPHICS_REG_ADDR, MAP_MASK_REGISTER);
    portByteOut(GRAPHICS_REG_DATA, readMapSelectRegister);
    portByteOut(GRAPHICS_REG_ADDR, GRAGHICS_MODE_REG);
    portByteOut(GRAPHICS_REG_DATA, graphicsModeRegister);
    portByteOut(GRAPHICS_REG_ADDR, MISC_GRAPHICS_REG);
    portByteOut(GRAPHICS_REG_DATA, miscGraphicsRegister);

	uint8_t fontIndex = 0;
    // Read the current value of the Character Map Select Register
    portByteOut(SEQUENCER_REG_ADDR, MAP_SELECT_REGISTER);
    uint8_t currentValue = portByteIn(SEQUENCER_REG_DATA);

    // Modify the register to select the new font
    currentValue = (currentValue & 0xFC) | (fontIndex & 0x03);

    // Write the modified value back to the Character Map Select Register
    portByteOut(SEQUENCER_REG_ADDR, MAP_SELECT_REGISTER);
    portByteOut(SEQUENCER_REG_DATA, currentValue);
}

void setFont(uint8_t *font, uint8_t fontHeight) {
    // Sequencer registers:
    uint8_t mapMaskRegister, memoryModeRegister;
    // Graphics registers:
    uint8_t readMapSelectRegister, graphicsModeRegister, miscGraphicsRegister;

    // Save current state of the sequencer registers
    portByteOut(0x3C4, 0x02); // Map Mask Register
    mapMaskRegister = portByteIn(0x3C5);

    portByteOut(0x3C4, 0x04); // Memory Mode Register
    memoryModeRegister = portByteIn(0x3C5);
    // Set flat addressing:
    portByteOut(0x3C5, memoryModeRegister | 0x04);

    // Save current state of the graphics registers
    portByteOut(0x3CE, 0x04); // Read Map Select Register
    readMapSelectRegister = portByteIn(0x3CF);

    portByteOut(0x3CE, 0x05); // Graphics Mode Register
    graphicsModeRegister = portByteIn(0x3CF);
    portByteOut(0x3CF, graphicsModeRegister & ~0x10); // Turn off even-odd addressing

    portByteOut(0x3CE, 0x06); // Miscellaneous Graphics Register
    miscGraphicsRegister = portByteIn(0x3CF);
    portByteOut(0x3CF, miscGraphicsRegister & ~0x02); // Turn off even-odd addressing

    // Write the font to plane P4 (assuming they are P1, P2, P4, P8)
    setVideoPlane(2);

    // Write to font 1:
	uint8_t *fontMemory = (uint8_t *)(0xB8000);
    for (uint16_t i = 0; i < 256; i++) {
        memcpy(fontMemory + i * 32, font, fontHeight);
        font += fontHeight;
    }
    // Restore the video plane
    setVideoPlane(0);

    // Restore registers:
    portByteOut(0x3C4, 0x02); // Map Mask Register
    portByteOut(0x3C5, mapMaskRegister);
    portByteOut(0x3C4, 0x04); // Memory Mode Register
    portByteOut(0x3C5, memoryModeRegister);
    portByteOut(0x3CE, 0x04); // Read Map Select Register
    portByteOut(0x3CF, readMapSelectRegister);
    portByteOut(0x3CE, 0x05); // Graphics Mode Register
    portByteOut(0x3CF, graphicsModeRegister);
    portByteOut(0x3CE, 0x06); // Miscellaneous Graphics Register
    portByteOut(0x3CF, miscGraphicsRegister);

    uint8_t fontIndex = 0;
    // Read the current value of the Character Map Select Register
    portByteOut(0x3C4, 0x03); // Character Map Select Register
    uint8_t currentValue = portByteIn(0x3C5);

    // Modify the register to select the new font
    currentValue = (currentValue & 0xFC) | (fontIndex & 0x03);

    // Write the modified value back to the Character Map Select Register
    portByteOut(0x3C4, 0x03); // Character Map Select Register
    portByteOut(0x3C5, currentValue);

}