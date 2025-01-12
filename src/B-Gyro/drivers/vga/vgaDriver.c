# include "arch/i386/ports/portsIO.h"
# include "drivers/vga.h"
# include "klibc/memory.h"

void setVideoPlane(uint8_t plane) {
    portByteOut(SEQUENCER_REG_ADDR, MAP_MASK_REGISTER);
    portByteOut(SEQUENCER_REG_DATA, 1 << plane);
}

uint8_t	g_previousDumps[61];
void savePreviousDumps(void) {
	uint8_t *val = g_previousDumps;
	*val = portByteIn(MISC_OUTPUT_REG_RE);
	val++;
	for (uint8_t i = 0; i < 5; i++) {
		portByteOut(SEQUENCER_REG_ADDR, i);
		*val = portByteIn(SEQUENCER_REG_DATA);
		val++;
	}
	for (uint8_t i = 0; i < 25; i++) {
		portByteOut(CRT_CTRL_REG_ADDR_D, i);
		*val = portByteIn(CRT_CTRL_REG_DATA_D);
		val++;
	}
	for (uint8_t i = 0; i < 9; i++) {
		portByteOut(GRAPHICS_REG_ADDR, i);
		*val = portByteIn(GRAPHICS_REG_DATA);
		val++;
	}
	for (uint8_t i = 0; i < 21; i++) {
		(void)portByteIn(FEATURE_CTRL_REG_COLOR);
		portByteOut(ATTRIBUTE_CTRL_REG_ADDR, i);
		*val = portByteIn(ATTRIBUTE_CTRL_REG_DATA);
		val++;
	}
	(void)portByteIn(FEATURE_CTRL_REG_COLOR);
	portByteOut(ATTRIBUTE_CTRL_REG_ADDR, 0x20);
}

void dumpToVGAPorts(uint8_t *val) {
	savePreviousDumps();
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


void	restorePreviousDumps(void){
	dumpToVGAPorts(g_previousDumps);
}

void setFont(uint8_t *font, uint8_t fontHeight){
	// sequencer registers:
	uint8_t mapMaskRegister, memoryModeRegister;
	// graphics registers:
	uint8_t readMapSelectRegister, graphicsModeRegister, miscGraphicsRegister;

	portByteOut(SEQUENCER_REG_ADDR, MAP_MASK_REGISTER);
	mapMaskRegister = portByteIn(SEQUENCER_REG_DATA);

	portByteOut(SEQUENCER_REG_ADDR, MEMORY_MODE_REGISTER);
	memoryModeRegister = portByteIn(SEQUENCER_REG_DATA);
	// set flat addressing:
	portByteOut(SEQUENCER_REG_DATA, memoryModeRegister | 0x04);

	portByteOut(GRAPHICS_REG_ADDR, READ_MAP_SELET_REG);
	readMapSelectRegister = portByteIn(GRAPHICS_REG_DATA);

	portByteOut(GRAPHICS_REG_ADDR, GRAGHICS_MODE_REG);
	graphicsModeRegister = portByteIn(GRAPHICS_REG_DATA);
	portByteOut(GRAPHICS_REG_DATA, graphicsModeRegister & ~0x10); 	// turn of even-odd addressing

	portByteOut(GRAPHICS_REG_ADDR, MISC_GRAPHICS_REG);
	miscGraphicsRegister = portByteIn(GRAPHICS_REG_DATA);
	portByteOut(GRAPHICS_REG_DATA, miscGraphicsRegister & ~0x02);		// turn of even-odd addressing

	setVideoPlane(2); // write the font to plane P4 (assuming they are P1, P2, P4, P8)

	// write to font 0:
	char *vgaMemory = (char *)0xA0000;
	for (uint16_t i = 0; i < 256; i++){
		 // Copy fontHeight bytes for each character
        memcpy(vgaMemory + i * 32, font + i * fontHeight, fontHeight);
        // Zero out the remaining bytes in the 32-byte character cell
        memset(vgaMemory + i * 32 + fontHeight, 0, 32 - fontHeight);
	}

	// restore registers:
	portByteOut(SEQUENCER_REG_ADDR, MAP_MASK_REGISTER);
	portByteOut(SEQUENCER_REG_DATA, mapMaskRegister);
	portByteOut(SEQUENCER_REG_ADDR, MEMORY_MODE_REGISTER);
	portByteOut(SEQUENCER_REG_DATA, memoryModeRegister);
	portByteOut(GRAPHICS_REG_ADDR, READ_MAP_SELET_REG);
	portByteOut(GRAPHICS_REG_DATA, readMapSelectRegister);
	portByteOut(GRAPHICS_REG_ADDR, GRAGHICS_MODE_REG);
	portByteOut(GRAPHICS_REG_DATA, graphicsModeRegister);
	portByteOut(GRAPHICS_REG_ADDR, MISC_GRAPHICS_REG);
	portByteOut(GRAPHICS_REG_DATA, miscGraphicsRegister);
}