#include "klibc/types.h"
#include "klibc/math.h"

#define PI 3.14159265358979323846

int floor(float x) {
    return (int)x - (x < 0 && x != (int)x);
}

// Taylor series approximation for sin(x) around 0 with extended terms.
static float enoughSin(float x) {
    float x2 = x * x;
    float x3 = x2 * x;
    float x5 = x3 * x2;
    float x7 = x5 * x2;
    float x9 = x7 * x2;

    return x - x3 / 6.0f + x5 / 120.0f - x7 / 5040.0f + x9 / 362880.0f;
}

// Approximate sine with proper range reduction and quadrant handling.
float sinApproximate(float x) {
    // Reduce x to the range [0, 2π)
    while (x < 0)
        x += 2 * PI;
    while (x >= 2 * PI)
        x -= 2 * PI;

    // Now x is between 0 and 2π.
    int k = floor(x * 2 / PI);  // Since x in [0, 2π), x*2/PI is in [0, 4)
    float y = x - k * (PI * 0.5f);       // y is the angle's offset within its quadrant

    // Using the quadrant, choose the correct form.
    switch (k % 4) {
        case 0: return enoughSin(y);
        case 1: return enoughSin(PI * 0.5f - y);
        case 2: return -enoughSin(y);
        case 3: return -enoughSin(PI * 0.5f - y);
        default: 
            return 0;  // Should never happen.
    }
}

float cosApproximate(float x) {
    return sinApproximate(x + PI * 0.5f);
}
