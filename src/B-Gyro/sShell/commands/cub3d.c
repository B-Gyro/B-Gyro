//#include "time/pit.h"
//#include "klibc/math.h"
//#include "drivers/vga.h"
//#include "klibc/print.h"
//#include "terminal/tty.h"
//#include "klibc/memory.h"
//#include "drivers/keyboard.h"

//typedef struct player{
//	_vec2 pos;
//	double dx;
//	double dy;
//	double angle;
//} _player;


//typedef struct cub3d{
//	bool isRunning;
//	_player player;
//	_vec2	camera;
//	uint16_t mapX;
//	uint16_t mapY;
//} _cub3dData;

//_cub3dData data;
//extern _vgaMode g_G320x200x256;

//# define CELL_WIDTH 8
//# define MAX_DOF 8
//# define PI2 PI/2
//# define PI32 3*PI/2

//char *grid[] = {
//	"11111111111111111111111111",
//	"10000000000000000000000001",
//	"1000001P100000000000000001",
//	"10000001000000000000000001",
//	"10000000000000010000000001",
//	"10000000000000101000000001",
//	"10000000000000101000000001",
//	"10000000000000000000000001",
//	"10000000000000000000000001",
//	"11111111111111111111111111",
//	NULL
//};

//uint8_t	getColor(char c){
//	switch (c){
//		case '1': return 1;
//		default : return 0;
//	}
//}

//void	drawPlayer(){
//	drawFilledSquare((_positionPair){.x = data.player.pos.x, .y = data.player.pos.y}, 1, 3);
//}

//void	displayGrid(bool	firstTime){
//	size_t i, j;

//	for (i = 0; grid[i]; i++){
//		for (j = 0; grid[i][j]; j++){
//			if (grid[i][j] == 'P'){
//				data.player.pos.x = j * CELL_WIDTH, data.player.pos.y = i * CELL_WIDTH;
//				grid[i][j] = '0';
//			}
//			drawFilledSquare((_positionPair){.x = j * CELL_WIDTH, .y = i * CELL_WIDTH}, CELL_WIDTH, getColor(grid[i][j]));
//		}
//	}
//	drawPlayer();
//	if (!firstTime)
//		return;
//	data.mapX = j;
//	data.mapY = i;
//}

//void	inputManager(_letter key, _scanCode scancode){
//	(void)scancode;
//	switch (key){
//		case 'q':
//			data.isRunning = FALSE;
//			break;
//		case 'w':
//			data.player.pos.x += data.player.dx;
//			data.player.pos.y += data.player.dy;
//			break;
//		case 's':
//			data.player.pos.x -= data.player.dx;
//			data.player.pos.y -= data.player.dy;
//			break;
//		case 'a':
//			data.player.angle -= 0.1;
//			if (data.player.angle < 0)
//				data.player.angle = 2 * PI;
//			goto APPLY_NEW_ANGLE;
//		case 'd':
//			data.player.angle += 0.1;
//			if (data.player.angle > 2 * PI)
//				data.player.angle = 0;
//			goto APPLY_NEW_ANGLE;
//		default: break;
//	}
//	if (scancode == ESCAPE)
//		data.isRunning = FALSE;
//	return ;
//	APPLY_NEW_ANGLE:
//	data.player.dx = cosApproximate(data.player.angle) * 5;
//	data.player.dy = sinApproximate(data.player.angle) * 5;
//}

//_positionPair getVerticalIntersaction(double rayAngle, _vec2 playerPosition){
//	double atan;

//	atan = 1 / tanApproximate(rayAngle);
//}

//_positionPair getHorizentalIntersaction(double rayAngle, _vec2 playerPosition){
//	double tan;

//	tan = tanApproximate(rayAngle);
//}

//void	castRays(){
//	_player *p = &data.player;

//	double		tan, atan, rayAngle;
//	uint32_t	dx, dy, raysCount, dof;
//	_positionPair	verticalInter, horizentalInter;

//	raysCount = 1;
//	dof = 0;
//	for (size_t r = 0; r < raysCount; r++){
//		rayAngle = data.player.angle;
//		verticalInter = getVerticalIntersaction(rayAngle, p->pos);
//		horizentalInter = getHorizentalIntersaction(rayAngle, p->pos);

//		drawLine(
//			(_positionPair){.x = p->pos.x, .y = p->pos.y},
//			verticalInter,
//			3
//		);
//		drawLine(
//			(_positionPair){.x = p->pos.x, .y = p->pos.y},
//			horizentalInter,
//			4
//		);
//	}
	
//}

//void	cub3d(char *args){
//	(void)args;
//	changeVGAMode13h();
//	keyboardSetKeyPressHandler(inputManager);

//	data.isRunning = TRUE;
//	data.player.angle = 0;
//	data.player.dx = cosApproximate(data.player.angle) * 5;
//	data.player.dy = sinApproximate(data.player.angle) * 5;
//	//double time = 0;
//	//double oldTime = 0;
//	//data.camera.x = 0;
//	//data.camera.y = 0.66; //=> POV = 2 * atan(0.66/1) = 66Deg
//	displayGrid(TRUE);
//	while (data.isRunning){
//		g_G320x200x256.clearScreen(TRUE);
//		displayGrid(FALSE);
//		castRays();
//		msleep(15);
//	}
//	changeVGAMode640x480x16();
//	keyboardResetKeyPressHandler();
//}