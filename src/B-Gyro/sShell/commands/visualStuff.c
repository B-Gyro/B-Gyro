#include "time/pit.h"
#include "klibc/math.h"
#include "drivers/vga.h"
#include "klibc/print.h"
#include "terminal/tty.h"
#include "klibc/memory.h"
#include "drivers/keyboard.h"

extern _vgaMode g_G320x200x256;
bool	running = TRUE;

typedef struct vec3{
	double x;
	double y;
	double z;
}_vec3;

typedef struct vec2{
	double x;
	double y;
} _vec2;

typedef struct connection{
	int a,b;
} _connection;

typedef struct shape{
	_vec3		*points;
	uint8_t		pointsNum;
	_connection	*connections;
	uint8_t		connectionsNum;
	_vec3		centroid;
} _shape;

void rotate(_vec3 *point, double x, double y, double z) {
    double original_x = point->x;
    double original_y = point->y;
    double original_z = point->z;
    
    // X rotation
    if (x != 0) {
        double cos_x = cosApproximate(x);
        double sin_x = sinApproximate(x);
        point->y = cos_x * original_y - sin_x * original_z;
        point->z = sin_x * original_y + cos_x * original_z;
        // Update originals for next rotation
        original_y = point->y;
        original_z = point->z;
    }
    
    // Y rotation
    if (y != 0) {
        double cos_y = cosApproximate(y);
        double sin_y = sinApproximate(y);
        point->x = cos_y * original_x + sin_y * original_z;
        point->z = -sin_y * original_x + cos_y * original_z;
        // Update original for next rotation
        original_x = point->x;
        original_z = point->z;
    }
    
    // Z rotation
    if (z != 0) {
        double cos_z = cosApproximate(z);
        double sin_z = sinApproximate(z);
        point->x = cos_z * original_x - sin_z * original_y;
        point->y = sin_z * original_x + cos_z * original_y;
    }
}


void	getUserInput(uint8_t letter){
	if (letter == 'q')
		running = FALSE;
}


void	makeCentroid(_shape *shape){
	shape->centroid.x = shape->centroid.y = shape->centroid.z = 0;
	for (size_t i = 0; i < shape->pointsNum; i++){
		shape->centroid.x += shape->points[i].x;
		shape->centroid.y += shape->points[i].y;
		shape->centroid.z += shape->points[i].z;
	}
	shape->centroid.x /= shape->pointsNum;
	shape->centroid.y /= shape->pointsNum;
	shape->centroid.z /= shape->pointsNum;
}

void	makeCube(_shape *cube, _vec3 *points, _connection *connections){
	cube->points = points;
	cube->pointsNum = 8;
	cube->connections = connections;
	cube->connectionsNum = 12;

	makeCentroid(cube);
}

void	makePyramid(_shape *pyramid, _vec3 *points, _connection *connections){
	pyramid->points = points;
	pyramid->pointsNum = 4;
	pyramid->connections = connections;
	pyramid->connectionsNum = 6;

	makeCentroid(pyramid);
}

void	drawShape(_shape *shape, uint8_t color){
	for (size_t i = 0; i < shape->connectionsNum; i++){
		drawLine((_positionPair){.x = shape->points[shape->connections[i].a].x, .y = shape->points[shape->connections[i].a].y},
				 (_positionPair){.x = shape->points[shape->connections[i].b].x, .y = shape->points[shape->connections[i].b].y}, color);
	}
}

void	rotateShape(_shape *shape, double x, double y, double z){
	for (size_t i = 0; i < shape->pointsNum; i++){
		shape->points[i].x -= shape->centroid.x;
		shape->points[i].y -= shape->centroid.y;
		shape->points[i].z -= shape->centroid.z;
		rotate(&(shape->points[i]), x, y, z);
		shape->points[i].x += shape->centroid.x;
		shape->points[i].y += shape->centroid.y;
		shape->points[i].z += shape->centroid.z;
	}
}

extern bool g_shellMode;

void	visualStuff(char *args){
	(void) args;

	_shape cube, pyramid;
	_vec3 cubePoints[] = {
		{25 + 50, 50, 50},
		{25 + 100, 50, 50},
		{25 + 100, 100, 50},
		{25 + 50, 100, 50},

		{25 + 50, 50, 100},
		{25 + 100, 50, 100},
		{25 + 100, 100, 100},
		{25 + 50, 100, 100},
	};
	_connection cubeConnections[12] = {
		{0, 4},
		{1, 5},
		{2, 6},
		{3, 7},

		{0, 1},
		{1, 2},
		{2, 3},
		{3, 0},

		{4, 5},
		{5, 6},
		{6, 7},
		{7, 4}
	};
	_vec3 pyramidPoints[] = {
		{45, 50, 50},
		{25, 100, 50},
		{65, 100, 50},

		{20, 100, 100}
	};

	_connection pyramidConnections[6] = {
		{0, 1},
		{0, 2},
		{0, 3},
		{1, 2},
		{2, 3},
		{1, 3}
	};
	makeCube(&cube, cubePoints, cubeConnections);
	makePyramid(&pyramid, pyramidPoints, pyramidConnections);

	g_shellMode = 0;
	changeVGAMode13h();
	keyboardSetKeyPressHandler(getUserInput);
	running = TRUE;
	while (running){
		rotateShape(&cube, 0.003, 0.05, 0.004);
		rotateShape(&pyramid, 0.005, 0.001, 0.001);
		drawShape(&cube, 3);
		drawShape(&pyramid, 2);
		msleep(1);
		g_G320x200x256.clearScreen(TRUE);
	}
	changeVGAMode640x480x16();
	g_shellMode = 1;
	putTtyBuffer();
	updateStatusBar();
	keyboardResetKeyPressHandler();
	// todo: we need to update the status bar !!!
}