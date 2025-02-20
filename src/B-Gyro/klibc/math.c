#include "klibc/math.h"

// Factorial function
size_t factorial(int n) {
    size_t fact = 1;
    for (int i = 2; i <= n; i++)
        fact *= i;
    return fact;
}

// Power function
double power(double base, int exp) {
    double result = 1.0;
    for (int i = 0; i < exp; i++)
        result *= base;
    return result;
}

// Taylor series approximation for sin(x)
double sinApproximate(double x) {
    double sum = 0;
    int terms = 10;  // More terms = more precision

    for (int i = 0; i < terms; i++) {
        int exponent = 2 * i + 1;
        double term = power(x, exponent) / factorial(exponent);
        sum += (i % 2 == 0) ? term : -term;
    }

    return sum;
}

double cosApproximate(double x) {
    return sinApproximate(x + (PI / 2.0));
}


// Convert degrees to radians
double degToRad(double degrees) {
    return degrees * (PI / 180.0);
}

//void testSinCosApproximate(){
//	double angle;
//	for (angle = 0; angle <= 90; angle += 5) {
//		double sin_val = sinApproximate(degToRad(angle));
//		double cos_val = cosApproximate(degToRad(angle));
//		printf("Angle: %f, sin: %f, cos: %f\n", angle, sin_val, cos_val);
//	}
//}

//int	main(){
//	// Test the sin and cos approximation functions
//	testSinCosApproximate();
//}