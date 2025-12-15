#include "klibc/math.h"

#define PI 3.14159265358979323846

int floor(double x) {
    return (int)x - (x < 0 && x != (int)x);
}

size_t pow(uint32_t base, uint32_t exp){
    size_t res = 1;
    
    while (exp > 0){
        res *= base;
        exp--;
    }

    return res;
}

// Taylor series approximation for sin(x) around 0 with extended terms.
static double enoughSin(double x) {
    double x2 = x * x;
    double x3 = x2 * x;
    double x5 = x3 * x2;
    double x7 = x5 * x2;
    double x9 = x7 * x2;

    return x - x3 / 6.0f + x5 / 120.0f - x7 / 5040.0f + x9 / 362880.0f;
}

// Approximate sine with proper range reduction and quadrant handling.
double sinApproximate(double x) {
    // Reduce x to the range [0, 2π)
    while (x < 0)
        x += 2 * PI;
    while (x >= 2 * PI)
        x -= 2 * PI;

    // Now x is between 0 and 2π.
    int k = floor(x * 2 / PI);  // Since x in [0, 2π), x*2/PI is in [0, 4)
    double y = x - k * (PI * 0.5f);       // y is the angle's offset within its quadrant

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

double cosApproximate(double x) {
    return sinApproximate(x + PI * 0.5f);
}

double tanApproximate(double x) {
    return sinApproximate(x) / cosApproximate(x);
}


// Convert degrees to radians
double degToRad(double degrees) {
    return degrees * (PI / 180.0);
}