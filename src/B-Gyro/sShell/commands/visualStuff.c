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


void testSinCosApproximate(){
	double angle;
	for (angle = 0; angle <= 90; angle += 5) {
		double rad = angle * (PI / 180.0f); // Convert degrees to radians
		double sin_val = sinApproximate(rad);
		double cos_val = cosApproximate(rad);
		SERIAL_DEBUG("Angle: %f, rad: %f, Sin: %f, Cos: %f", angle, rad, sin_val, cos_val);
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