#include "arch/i386/pit.h"
#include "klibc/print.h"
#include "drivers/vga.h"



char *map[] = {
	"111111111111111111111",
	"100000000000100000001",
	"100000000000100000001",
	"100000111111111110001",
	"1000000000P0000000001",
	"100000000000000000001",
	"111111111111111111111",
};

typedef enum gameState {
	NORMAL,
	QUIT,
} e_gameState;

typedef struct gameData {
	e_gameState	state;
	_vgaMode	*vm;
	_positionPair player;
} _gameData;


size_t ticks;

_gameData g_gd = {
	.state = NORMAL,
	.vm = NULL
};

void	onKeyPress(letter key){
	int x, y;

	x = y = 0;
	switch (key)
	{
	case 'w':
		y--;
		break;
	case 'a':
		x--;
		break;
	case 's':
		y++;
		break;
	case 'd':
		x++;
		break;
	default:
		return ;
		break;
	}
	if (map[g_gd.player.y + y][g_gd.player.x + x] == '0'){
		map[g_gd.player.y + y][g_gd.player.x + x] = 'P';
		map[g_gd.player.y][g_gd.player.x] = '0';
	}
}

void	onKeyRelease(scanCode key){
	(void)key;

}


static void	drawSquare(uint32_t x, uint32_t y, uint32_t sideLen, uint8_t color){
	for (size_t i = 0; i < sideLen; i++){
		for (size_t j = 0; j < sideLen; j++){
			g_gd.vm->putPixel((struct positionPair){x + j, y + i}, color);
		}
	}	
}

// https://www.youtube.com/watch?v=hpiILbMkF9w

void drawCircle(uint32_t centerX, uint32_t centerY, uint16_t r, uint8_t color){
	uint32_t x, y, rSquared;

	x = 0;
	y = -r;
	rSquared = r*r;
	while (x < -y){

		if (x*x + y*y > rSquared)
			y++;
		g_gd.vm->putPixel((struct positionPair){centerX + x, centerY + y}, color);
		g_gd.vm->putPixel((struct positionPair){centerX - x, centerY + y}, color);
		g_gd.vm->putPixel((struct positionPair){centerX + x, centerY - y}, color);
		g_gd.vm->putPixel((struct positionPair){centerX - x, centerY - y}, color);
		g_gd.vm->putPixel((struct positionPair){centerX + y, centerY + x}, color);
		g_gd.vm->putPixel((struct positionPair){centerX + y, centerY - x}, color);
		g_gd.vm->putPixel((struct positionPair){centerX - y, centerY + x}, color);
		g_gd.vm->putPixel((struct positionPair){centerX - y, centerY - x}, color);
		x++;
	}	
}

static void drawRhombus(uint32_t centerX, uint32_t centerY, uint16_t r, uint8_t color){
	uint32_t x, y, p;

	x = 0;
	y = -r;
	p = -r;
	while (x < -y){

		if (p > 0){
			y++;
			p += 2 * (x + y) + 1;
		}
		else
			p += 2*x + 1;
		g_gd.vm->putPixel((struct positionPair){centerX + x, centerY + y}, color);
		g_gd.vm->putPixel((struct positionPair){centerX - x, centerY + y}, color);
		g_gd.vm->putPixel((struct positionPair){centerX + x, centerY - y}, color);
		g_gd.vm->putPixel((struct positionPair){centerX - x, centerY - y}, color);
		g_gd.vm->putPixel((struct positionPair){centerX + y, centerY + x}, color);
		g_gd.vm->putPixel((struct positionPair){centerX + y, centerY - x}, color);
		g_gd.vm->putPixel((struct positionPair){centerX - y, centerY + x}, color);
		g_gd.vm->putPixel((struct positionPair){centerX - y, centerY - x}, color);
		x++;
	}	
}


void	drawGrid(){
	uint8_t color, sideLen, raduis;

	sideLen = 10;
	raduis = sideLen/2;
	for (size_t y = 0; y < 7; y++) {
		for (size_t x = 0; x < 21; x++) {
			color = map[y][x] == '1' ? 0x02 : 0x00;
			drawSquare(x * sideLen, y * sideLen, sideLen, color);
			if (map[y][x] == 'P'){
				drawRhombus(x * sideLen + raduis, y * sideLen + raduis, raduis, 0x03);
				g_gd.player.x = x;	
				g_gd.player.y = y;	
			}
		}
	}
}

static void	clearScreen(_vgaMode *vgaMode){
	for (size_t i = 0; i < vgaMode->screenHeight; i++)
	{
		for (size_t j = 0; j < vgaMode->screenWidth; j++){
			_positionPair pos = {j, i};
			vgaMode->putPixel(pos, 0);
		}
	}
}

void	Nothing(_registers r){
	(void)r;
	ticks++;
}

void wait(){
	while (ticks % 10)
	{
		/* code */
	}
}

void	initCub3d(char *args){
	(void)args;
	// init video mode
	g_gd.vm = changeVGAMode13h();
	// init keyboard handler
	keyboardSetKeyPressHandler(onKeyPress);
	keyboardSetKeyReleaseHandler(onKeyRelease);
	setIRQHandler(TIMER_IRQ, Nothing);
	// init game data

	while(1){
		clearScreen(g_gd.vm);
		drawGrid();
		//drawCircle(50, 50, 20, 0x03);
		//drawRhombus(100, 50, 20, 0x03);
		wait();
	}
	//while (g_gd.state != QUIT) {
	//	// proccess events
	//	// tickGame
	//	// write to pixel buffer
	//	// write to vga buffer
	//}
	
}