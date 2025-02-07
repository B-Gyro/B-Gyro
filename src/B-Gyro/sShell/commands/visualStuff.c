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
	float angle;
	for (angle = 0; angle <= 90; angle += 5) {
		float rad = angle * (PI / 180.0f); // Convert degrees to radians
		float sin_val = sinApproximate(rad);
		float cos_val = cosApproximate(rad);
		SERIAL_PRINT("Angle: %f, rad: %f, Sin: %f, Cos: %f\n", angle, rad, sin_val, cos_val);
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