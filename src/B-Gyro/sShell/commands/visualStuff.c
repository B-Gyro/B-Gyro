#include "klibc/memory.h"
#include "klibc/print.h"
#include "drivers/keyboard.h"
#include "drivers/vga.h"
#include "klibc/math.h"
#include "time/pit.h"

extern _vgaMode g_G320x200x256;

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

void	visualStuff(char *args){

	(void) args;
	_vec3 points[8] = {
		{25 + 50, 50, 50},
		{25 + 100, 50, 50},
		{25 + 100, 100, 50},
		{25 + 50, 100, 50},

		{25 + 50, 50, 100},
		{25 + 100, 50, 100},
		{25 + 100, 100, 100},
		{25 + 50, 100, 100},
	};
	
	_connection connections[12] = {
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

	_vec3 centroid = {0};
	for (size_t i = 0; i < 8; i++){
		centroid.x += points[i].x;
		centroid.y += points[i].y;
		centroid.z += points[i].z;
	}
	centroid.x /= 8;
	centroid.y /= 8;
	centroid.z /= 8;
	changeVGAMode13h();

	while (1){
		for (size_t i = 0; i < 8; i++){
			points[i].x -= centroid.x;
			points[i].y -= centroid.y;
			points[i].z -= centroid.z;
			rotate(&(points[i]), 0.003, 0.001, 0.004);
			points[i].x += centroid.x;
			points[i].y += centroid.y;
			points[i].z += centroid.z;
			g_G320x200x256.putPixel((_positionPair){.x = points[i].x, .y = points[i].y}, 15);
		}
		for (size_t i = 0; i < 12; i++){
			drawLine((_positionPair){.x = points[connections[i].a].x, .y = points[connections[i].a].y},
					 (_positionPair){.x = points[connections[i].b].x, .y = points[connections[i].b].y}, 15);
		}
		msleep(1);
		g_G320x200x256.clearScreen(TRUE);
	}
	changeVGAMode640x480x16();
}