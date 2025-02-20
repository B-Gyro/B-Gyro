#include "klibc/memory.h"
#include "klibc/print.h"
#include "drivers/keyboard.h"
#include "drivers/vga.h"
#include "klibc/math.h"
#include "time/pit.h"

extern _vgaMode g_G320x200x256;

typedef struct vec3{
	float x;
	float y;
	float z;
}_vec3;


void testSinCosApproximate(){
	double angle;
	for (angle = 0; angle <= 90; angle += 5) {
		double sin_val = sinApproximate(degToRad(angle));
		double cos_val = cosApproximate(degToRad(angle));
		SERIAL_DEBUG("Angle: %f, sin: %f, cos: %f", angle, sin_val, cos_val);
	}
}


void	visualStuff(char *args){

	(void) args;
	changeVGAMode13h();
	//asm volatile("cli");

	testSinCosApproximate();

	while (1);
	changeVGAMode640x480x16();
}